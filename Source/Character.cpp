#include"Character.h"
#include"StageManager.h"
#include"Mathf.h"

//更新行列
void Character::UpdateTranceform() {
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//回転行列を作成

	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;

	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//３つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	//計算したワールドを取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

//移動処理
void Character::Move(float vx, float vz, float speed) {
	//横方向ベクトルを設定
	moveVecX = vx;
	moveVecZ = vz;
	//最大速度設定
	maxMoveSpeed = speed;
}

//旋回処理
void Character::Turn(float elapsedTime, float vx, float vz, float speed) {
	speed *= elapsedTime;
	//進行ベクトルがゼロベクトルの場合は処理する必要無し
	if (vx == 0.0f && vz == 0.0f)return;
	float lenth = sqrtf(vx * vx + vz * vz);

	//進行ベクトルを単位ベクトル化
	vx /= lenth;
	vz /= lenth;

	//自身の回転値から前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	//回転角を求めるため、２つの単位ベクトルの内積を計算する
	float dot = (vx * frontX) + (vz * frontZ); //内積

	//内積値は-1.0~1.0で表現されており、２つの単位ベクトルの角度が
	//小さいほど1.0に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot; //補正値
	if (rot > speed) rot = speed;

	//左右判定を行うために２つの単位ベクトルの外積を計算する
	float cross = (vz * frontX) - (vx * frontZ);

	//2Dの外積値が正の場合か負の場合によって左右判定を行える
	//左右判定を行う事によって左右回転を選択する
	if (cross < 0.0f)
	{
		angle.y += rot;
	}
	else
	{
		angle.y -= rot;
	}
}

//ジャンプ処理
void Character::Junp(float speed) {
	//上方向の力を設定
	velocity.y = speed;
}

//衝撃を与える
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	//速力を力に加える
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}

//速力処理更新
void Character::UpdateVelocity(float elapsedTime) {
	//経過フレーム
	float elapsedFrame = 60.0f * elapsedTime;

	//垂直速力更新処理
	UpdateVerticalVelocity(elapsedFrame);
	//水平速力更新処理
	UpdateHorizontalVelocity(elapsedFrame);
	//垂直移動更新処理
	UpdateVerticalMove(elapsedTime);
	//水平移動更新処理
	UpdateHorizontalMove(elapsedTime);
}

bool Character::ApplyDamage(int damage,float invincibleTime)
{
	//ダメージが０の場合は健康状態を変更する必要がない
	if (damage == 0) return false;
	//死亡している場合は健康状態を変更しない
	if (health < 0) return false;
	//ダメージ処理
	if (invincibleTime > 0) {
		health -= damage;
		//死亡通知
		if (health <= 0) {
			OnDead();
		}
		//ダメージ通知
		else {
			OnDamaged();
			invicibleTime = invincibleTime;
		}
	}
	//健康状態が変更した場合はtrueを返す
	return true;
}

void Character::UpdateInvinciblTImer(float elapsedTime)
{
	if (invicibleTime > 0.0f) {
		invicibleTime -= elapsedTime;
	}
}

//垂直速力更新処理
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
	//重力処理
	velocity.y += gravity * elapsedFrame;;
}

//垂直移動更新処理
void Character::UpdateVerticalMove(float elapsedTime)
{
	//垂直方向の移動量
	float my = velocity.y * elapsedTime;

	slopeRate = 0.0f;
	//キャラクターのY軸方向となる法線ベクトル
	DirectX::XMFLOAT3 normal = { 0,1,0 };

	//落下中
	if (my < 0.0f)
	{
		//レイの開始位置は足元より少し上
		DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
		//レイの終点位置は移動後の位置
		DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };

		//レイキャストによる地面判定
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			//法線ベクトルを取得
			normal = hit.normal;

			//地面に接地している
			position = hit.position;
			//回転
			angle.y += hit.rotation.y;
			//傾斜の計算
			slopeRate = 1.0f - (hit.normal.y /
				(hit.normal.y + sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z)));

			//着地した
			if (!isGround)
			{
				OnLanding();
			}
			isGround = true;
			velocity.y = 0.0f;


		}
		else
		{
			//空中に浮いている
			position.y += my;
			isGround = false;
		}
	}
	//上昇中
	else if (my > 0.0f)
	{
		position.y += my;
		isGround = false;
	}

	//地面の向きに沿うようにXZ軸回転
	{
		//Y軸が法線ベクトル方向に向くオイラー角回転を算出する

		float angleX = atan2f(normal.z, normal.y);
		float angleZ = -atan2f(normal.x, normal.y);

		//線形補完で滑らかに回転する
		if (angleX <= 1.0f && angleX >= -1.0f) {
			angle.x = Mathf::Leap(angle.x, angleX, elapsedTime * 10.0f);
		}
		if (angleZ <= 1.0f && angleZ >= -1.0f) {
			angle.z = Mathf::Leap(angle.z, angleZ, elapsedTime * 10.0f);
		}
	}
}

//水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	//XZ平面の速力を減速する
	//三平方で長さを取ろう
	float length = sqrtf((velocity.x * velocity.x) + (velocity.z * velocity.z));
	if (length > 0.0f) 
	{
		//摩擦力
		float friction = this->friction * elapsedFrame;
		//空中にいるときは摩擦力を減らす
		if (!IsGround()) friction -= this->airControl;

		//摩擦による横方向の減速処理
		if(length > friction)
		{
			//単位ベクトル化
			float vx = velocity.x / length;
			float vz = velocity.z / length;
			//単位ベクトル化した速力を摩擦力分スケーリングした値を速力から引く
			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		//横方向の速力が摩擦力以下になったので速力を無効化
		else
		{
			velocity.x = 0;
			velocity.z = 0;
		}
	}

	//XZ平面の速力を加速する
	if (length <= maxMoveSpeed)
	{
		//移動ベクトルがゼロベクトルでないなら加速する
		float moveVecLength = sqrtf((moveVecX * moveVecX) + (moveVecZ * moveVecZ));
		if(moveVecLength > 0.0f)
		{
			//加速力
			float acceleration = this->acceleration * elapsedFrame;
			//空中にいるときは加速力を減らす
			if (!IsGround()) acceleration -= this->airControl;
			//移動ベクトルによる加速処理
			velocity.x += moveVecX * acceleration;
			velocity.z += moveVecZ * acceleration;

			//最大速度制限
			float length = sqrtf((velocity.x * velocity.x) + (velocity.z * velocity.z));
			if (length > maxMoveSpeed)
			{
				//最大速度を超えたら速度を単位ベクトル（１）に最大速度をかけて整える
				velocity.x *= maxMoveSpeed / length;
				velocity.z *= maxMoveSpeed / length;
			}
		}

		//下りガタガタしないようにする
		if (IsGround() && slopeRate > 0.0f)
		{
			velocity.y -= length * slopeRate;
		}
	}
	//移動ベクトルをリセット
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

//水平移動更新処理
void Character::UpdateHorizontalMove(float elapsedTime)
{
	//水平速力計算
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		//水平移動
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;

		//レイの開始位置と終点位置
		DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
		DirectX::XMFLOAT3 end = { position.x + mx,position.y + stepOffset,position.z + mz };

		//レイキャストによる壁判定
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			//壁までのベクトル
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
			//壁の法線
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			//入射ベクトルを法線に射影
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

			//補正位置の計算
			float distance = 0;
			DirectX::XMStoreFloat(&distance, Dot);

			//法線の大きさを距離に合わせる
			DirectX::XMVECTOR R = DirectX::XMVectorSubtract(Vec, DirectX::XMVectorScale(Normal, distance));

			DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
			Position = DirectX::XMVectorAdd(Position, R);
			DirectX::XMStoreFloat3(&position, Position);
		}
		else 
		{
			//移動
			position.x += mx;
			position.z += mz;
		}

	}
}