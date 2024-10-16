#include"Sperm.h"
#include"Mathf.h"
#include"Collision.h"
#include"Player.h"
#include"Sperm_Manager.h"

Sperm_child::Sperm_child()
{
	scale.x = scale.y = scale.z = 0.05f;
	scale.x = scale.z *= -1;
	mdl = std::make_unique<Model>("Data/Model/Player/player.mdl");
	radius = 0.5;
	height = 1.0;
	mdl->PlayAnimation(0, true, 0.2f);
}

Sperm_child::~Sperm_child()
{

}

void Sperm_child::Update(float& elapsedTime)
{
	// isActive か何かのフラグを用意して isActive が false なら return
	if (!isActive)return;

	switch (state)
	{
	case State::Wander:
		UpdateWanderState(elapsedTime);
		break;
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;
	case State::Follow:
		UpdateFollowState(elapsedTime);
		break;
	case State::Attack:
		UpdateAttack(elapsedTime);
		break;
	}

	mdl->UpdateAnimation(elapsedTime);
	//速力更新処理
	UpdateVelocity(elapsedTime);
	//無敵時間の更新
	UpdateInvinciblTImer(elapsedTime);
	//オブジェクト行列を更新
	UpdateTranceform();
	mdl->UpdateTransform(transform);
}

void Sperm_child::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	// isActive か何かのフラグを用意して isActive が false なら return
	if (!isActive)return;
	mdl->mask.dissolveThreshold = mask.dissolveThreshold;
	mdl->mask.edgColor = mask.edgColor;
	mdl->mask.edgThreshold = mask.edgThreshold;
	shader->Draw(dc, mdl.get());
}

void Sperm_child::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

void Sperm_child::SetRandomTargetPosition()
{
	float Angle = DirectX::XMConvertToRadians(Mathf::RandomRange(0, 360));
	float Length = Mathf::RandomRange(0, territoryRange);
	targetPosition.x = Mathf::RandomRange(territoryOrigin.x, territoryRange) + sinf(Angle) * Length;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = Mathf::RandomRange(territoryOrigin.z, territoryRange) + cosf(Angle) * Length;
}

void Sperm_child::MoveToTarget(const float& elapsedTime, float speedRate)
{
	//ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	vx /= dist;
	vy /= dist;
	vz /= dist;
	//移動処理
	Move(vx, vz, moveSpeed * speedRate,vy);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

void Sperm_child::TransitionWanderState()
{
	state = State::Wander;

	//目標地点へ移動
	SetRandomTargetPosition();
}

void Sperm_child::UpdateWanderState(float elapsedTime)
{
	//目標地点までXZ平面での距離判定
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius)
	{
		//待機ステートへ遷移
		TransitionIdleState();
	}
	//目標地点へ
	MoveToTarget(elapsedTime, 0.5f);
}

void Sperm_child::TransitionIdleState()
{
	state = State::Idle;

	//タイマーをランダム設定
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	//待機アニメーション再生
	//model->PlayAnimation(Anime_IdleNormal, true, 0.2f);
}

//待機ステート更新処理
void Sperm_child::UpdateIdleState(float elapsedTime)
{
	//タイマー処理
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		if (player_catch)
		{
			//追跡ステートへ
			TransitionFollowState();
		}
		else
		{
			//徘徊ステートへ遷移
			TransitionWanderState();
		}
	}
}

void Sperm_child::TransitionFollowState()
{
	state = State::Follow;
}

void Sperm_child::UpdateFollowState(float elapsedTime)
{
	targetPosition = Player::Instance().GetPosition();
	//目標地点へ移動
	MoveToTarget(elapsedTime, 5.0f);

	DirectX::XMVECTOR vPos =  DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPosition), DirectX::XMLoadFloat3(&position));
	float dist = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(vPos));

	if (dist < stopRange)
	{
		//待機ステートへ遷移
		TransitionIdleState();
	}
}

void Sperm_child::TransitionAttackState()
{
	state = State::Attack;
	if (Player::Instance().Get_Target_Enemy())
	{
		targetPosition = Player::Instance().Get_Target_Enemy()->GetPosition();
	}
	else
	{
		DirectX::XMFLOAT4X4 player_transform = Player::Instance().Get_Transform();
		targetPosition.x = player_transform._31;
		targetPosition.y = player_transform._32;
		targetPosition.z = player_transform._33;

		DirectX::XMStoreFloat3(&targetPosition, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&targetPosition)));
		DirectX::XMStoreFloat3(&targetPosition, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&targetPosition), 30.0f));
	}
}

void Sperm_child::UpdateAttack(float elapsedTime)
{
#if  1
	MoveToTarget(elapsedTime, 5.0f);
	DirectX::XMVECTOR vPos = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPosition), DirectX::XMLoadFloat3(&position));
	float dist = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(vPos));

	if (dist < stopRange)
	{
		//Sperm_Manager::Instance().Remove(this);
		// Remove じゃなくて isActive を false にする
		isActive = false;
	}
#else
	Sperm_Manager::Instance().Remove(this);
#endif 
}