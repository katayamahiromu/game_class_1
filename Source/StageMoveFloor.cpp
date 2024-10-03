#include"StageMoveFloor.h"

//コンストラクタ
StageMoveFloor::StageMoveFloor()
{
	scale.x = scale.z = 3.0f;
	scale.y = 0.5;

	//ステージモデルを読み込み
	model = new Model("Data/Model/Cube/Cube.mdl");
}

StageMoveFloor::~StageMoveFloor()
{
	//ステージモデルを破棄
	delete model;
}

//更新処理
void StageMoveFloor::Update(float elapsedTime)
{
	//前回の情報を保存
	OldTransform = transform;

	//スタートからゴールまでの距離を算出する
	DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
	DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
	float length;
	DirectX::XMStoreFloat(&length, Length);
	//スタートからゴールまでの間を一秒間で進む割合（0.0~1.0）を算出する
	float speed = moveSpeed * elapsedTime;
	float speedRate = speed / length;
	moveRate += speedRate;


	//スタートからゴールまでの間を一秒間で進む場合、移動方向を反転させる
	if (moveRate <= 0.0f || moveRate >= 1.0f)
	{
		moveSpeed = -moveSpeed;
	}

	//線形補完で位置を算出する
	DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
	DirectX::XMStoreFloat3(&position, Position);

	//回転
	oldAngle = { angle.x,angle.y,angle.z };
	angle.x += torque.x * elapsedTime;
	angle.y += torque.y * elapsedTime;
	angle.z += torque.z * elapsedTime;

	//行列更新
	UpdateTransform();

	//レイキャスト用にモデル空間にするための単位行列を渡す
	const DirectX::XMFLOAT4X4 tranceformIdentity = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	model->UpdateTransform(tranceformIdentity);
}

//描画処理
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	//表示用のためのワールド行列に更新する
	model->UpdateTransform(transform);
	shader->Draw(dc, model);
}

//レイキャスト
bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start,const DirectX::XMFLOAT3& end,HitResult& hit )
{
	//前回のワールド行列と逆行列を求める
	DirectX::XMMATRIX OldMatrix = DirectX::XMLoadFloat4x4(&OldTransform);
	DirectX::XMMATRIX InverseMatrix = DirectX::XMMatrixInverse(nullptr, OldMatrix);

	//前回のローカル空間でのレイに変換
	DirectX::XMVECTOR startLocal = DirectX::XMVector3TransformCoord(
		DirectX::XMLoadFloat3(&start), InverseMatrix);
	DirectX::XMVECTOR endLocal = DirectX::XMVector3TransformCoord(
		DirectX::XMLoadFloat3(&end), InverseMatrix);


	//ローカル空間でのレイとの交点を求める
	DirectX::XMFLOAT3 localStart, localEnd;
	DirectX::XMStoreFloat3(&localStart, startLocal);
	DirectX::XMStoreFloat3(&localEnd, endLocal);
	HitResult localHit;
	if (Collision::InterserctRayVsModel(localStart, localEnd, model, localHit))
	{

		//前回の情報を今回の位置に取り入れる
		DirectX::XMMATRIX WorldTransform;
		WorldTransform = DirectX::XMLoadFloat4x4(&transform);
		//前回のローカル空間から今回のワールド空間へ変換
		//前回から今回にかけて変更された内容が乗っているオブジェクトに反映される
		DirectX::XMVECTOR hitPointLocal = DirectX::XMLoadFloat3(&localHit.position);
		DirectX::XMVECTOR hitPointWorld = DirectX::XMVector3TransformCoord(hitPointLocal, WorldTransform);
		DirectX::XMStoreFloat3(&hit.position, hitPointWorld);

		// 法線ベクトルの変換
		DirectX::XMVECTOR normalLocal = DirectX::XMLoadFloat3(&localHit.normal);
		DirectX::XMVECTOR normalWorld = DirectX::XMVector3TransformNormal(normalLocal, WorldTransform);
		DirectX::XMStoreFloat3(&hit.normal, normalWorld);

		//回転差分を算出
		hit.rotation.x = angle.x - oldAngle.x;
		hit.rotation.y = angle.y - oldAngle.y;
		hit.rotation.z = angle.z - oldAngle.z;

		return true;
	}
	return false;
}

//行列更新処理
void StageMoveFloor::UpdateTransform()
{
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMMATRIX W = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, W);
}