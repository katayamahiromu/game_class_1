#include"ProjectileHoming.h"

//コンストラクタ
ProjectileHoming::ProjectileHoming(ProjectileManager* manager) 
	:Projectile(manager)
{
	model = new Model("Data/Model/Sword/Sword.mdl");
	//モデルが小さいのでスケーリング
	scale.x = scale.y = scale.z = 3.0f;
}

//デストラクタ
ProjectileHoming::~ProjectileHoming()
{
	delete model;
}

//更新処理
void ProjectileHoming::Update(float elapsedTime)
{
	//寿命処理
	lifeTimer -= elapsedTime;
	if (lifeTimer < 0) {
		//自分を削除
		Destroy();
	}

	//移動
	float moveSpeed = this->moveSpeed * elapsedTime;
	position.x += direction.x * moveSpeed;
	position.z += direction.z * moveSpeed;

	//旋回
	float turnSpeed = this->turnSpeed * elapsedTime;
	//ターゲットまでのベクトルを算出
	DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
	DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

	//ゼロベクトルでないなら回転処理
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3Length(Vec);
	float lengthsq;
	DirectX::XMStoreFloat(&lengthsq, LengthSq);
	if(lengthsq > 0.00001f)
	{
		//ターゲットまでのベクトルを単位ベクトル化
		Vec = DirectX::XMVector3Normalize(Vec);

		//向いている方向ベクトルを算出
		DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

		//前方方向ベクトルとターゲットまでのベクトルの内積（角度）を算出
		DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);
		float dot;
		DirectX::XMStoreFloat(&dot, Dot);
		//２つの単位ベクトルの角度が小さいほど
		//1.0に近づくという性質を利用して回転速度を調整する
		float rot = 1.0f - dot; //補正値
		if (rot > turnSpeed) rot = turnSpeed;

		//回転角度があるなら回転処理をする
		if (turnSpeed > 0)
		{
			//回転軸を算出
			DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);

			//回転軸と回転量から回転行列を算出
			DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);

			//現在の回転行列を回転させる
			DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
			Transform = DirectX::XMMatrixMultiply(Rotation, Transform);

			//回転後の前方向ベクトルを取り出し、単位ベクトル化する
			Direction = DirectX::XMVector3TransformCoord(Direction, Rotation);
			DirectX::XMStoreFloat3(&direction, Direction);
		}




		//オブジェクト行列を更新
		UpdateTransform();
		//モデル行列更新
		model->UpdateTransform(transform);
	}
}

//描画処理
void ProjectileHoming::Render(ID3D11DeviceContext* dc, Shader* sheder)
{
	sheder->Draw(dc, model);
}

//発射
void ProjectileHoming::Launch(const DirectX::XMFLOAT3& direction,
	const DirectX::XMFLOAT3& posistion,
	const DirectX::XMFLOAT3& target)
{
	this->direction = direction;
	this->position = posistion;
	this->target = target;

	UpdateTransform();
}