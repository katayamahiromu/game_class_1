#include"Sperm.h"
#include"Mathf.h"
#include"Collision.h"
#include"Player.h"
#include"Sperm_Manager.h"
#include"EnemyManeger.h"
#include<algorithm>

Sperm_child::Sperm_child()
{
	isActive = true;
	FLT = false;
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
	// isActive �������̃t���O��p�ӂ��� isActive �� false �Ȃ� return
	/*const float min = 40;
	const float max = 300;
	std::clamp(position.y, min, max);
	if (position.y < 40.0f)position.y = 40.0f;*/
	//Update_ResPornTime(elapsedTime);
	if (isActive == false )return;

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
	case State::Daed:
		Dead(elapsedTime);
		break;
	}

	mdl->UpdateAnimation(elapsedTime);
	//���͍X�V����
	UpdateVelocity(elapsedTime);
	//���G���Ԃ̍X�V
	UpdateInvinciblTImer(elapsedTime);
	//�I�u�W�F�N�g�s����X�V
	UpdateTranceform();
	mdl->UpdateTransform(transform);
}

void Sperm_child::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	// isActive �������̃t���O��p�ӂ��� isActive �� false �Ȃ� return
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
	//�^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	vx /= dist;
	vy /= dist;
	vz /= dist;
	//�ړ�����
	Move(vx, vz, moveSpeed * speedRate,vy);
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

void Sperm_child::TransitionWanderState()
{
	state = State::Wander;

	//�ڕW�n�_�ֈړ�
	SetRandomTargetPosition();
}

void Sperm_child::UpdateWanderState(float elapsedTime)
{
	//�ڕW�n�_�܂�XZ���ʂł̋�������
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius)
	{
		//�ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}
	//�ڕW�n�_��
	MoveToTarget(elapsedTime, 0.5f);
}

void Sperm_child::TransitionIdleState()
{
	state = State::Idle;

	//�^�C�}�[�������_���ݒ�
	stateTimer = Mathf::RandomRange(0.0f, 1.0f);

	//�ҋ@�A�j���[�V�����Đ�
	//model->PlayAnimation(Anime_IdleNormal, true, 0.2f);
}

//�ҋ@�X�e�[�g�X�V����
void Sperm_child::UpdateIdleState(float elapsedTime)
{
	//�^�C�}�[����
	stateTimer -= elapsedTime;
	if (stateTimer < 0.0f)
	{
		if (player_catch)
		{
			//�ǐՃX�e�[�g��
			TransitionFollowState();
		}
		else
		{
			//�p�j�X�e�[�g�֑J��
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
	//�ڕW�n�_�ֈړ�
	MoveToTarget(elapsedTime, 5.0f);

	DirectX::XMVECTOR vPos =  DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPosition), DirectX::XMLoadFloat3(&position));
	float dist = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(vPos));

	if (dist < stopRange)
	{
		//�ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}
}

void Sperm_child::TransitionAttackState()
{
	state = State::Attack;
	if (Player::Instance().Get_Target_Enemy())
	{
		targetPosition = Player::Instance().Get_Target_Enemy()->GetPosition();
		targetPosition.y += 1.0f;
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
	MoveToTarget(elapsedTime, 5.0f);
	DirectX::XMVECTOR vPos = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&targetPosition), DirectX::XMLoadFloat3(&position));
	float dist = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(vPos));

	if (dist < stopRange)
	{
		//Sperm_Manager::Instance().Remove(this);
		// Remove ����Ȃ��� isActive �� false �ɂ���
		TransitionDead();

		EnemeyManager& enemyManager = EnemeyManager::Instance();
		//�S�Ă̓G�Ƒ�������ŏՓˏ���
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0;i < enemyCount;++i)
		{
			Enemy* enemy = enemyManager.GetEnemy(i);
			DirectX::XMFLOAT3 outPosition;
			DirectX::XMFLOAT3 pos = enemy->GetPosition();
			pos.y += 1.0f;
			if (Collision::IntersectSphereVsSphere(
				position,
				radius,
				pos,
				enemy->GetRadius(),
				outPosition
			))
			{
				if(enemy->GetHealth() > 0)enemy->ApplyDamage(1.0f, 1.0f);
			}
		};
	}
}

void Sperm_child::TransitionDead()
{
	state = State::Daed;
}

void Sperm_child::Dead(float elapsedTime)
{
	mask.dissolveThreshold = Mathf::Leap(mask.dissolveThreshold, 0.0f, elapsedTime);
	if (mask.dissolveThreshold <= 0.1f)isActive = false;
}

void Sperm_child::ResPornTransition()
{
	/*isActive = true;
	ResPornTime = 3.0f;
	state = State::Wander;
	targetPosition = Player::Instance().GetPosition();

	targetPosition.x += (rand() % 31) - 15;
	targetPosition.z += (rand() % 31) - 15;

	position = targetPosition;
	mask.dissolveThreshold = 1.0;*/
	Sperm_child* sc = new Sperm_child;
	DirectX::XMFLOAT3 pos = Player::Instance().GetPosition();
	pos.x += (rand() % 31) - 15;
	pos.y += (rand() % 31) - 15;
	pos.z += (rand() % 31) - 15;
	sc->SetPositon(pos);
	Sperm_Manager::Instance().RegisterAdd(sc);
}

void Sperm_child::Update_ResPornTime(float elapsedTime)
{
	if (isActive == true)return;
	ResPornTime -= elapsedTime;
	if (ResPornTime < 0.0f)
	{
		if (FLT == false)
		{
			//ResPornTransition();
			FLT = true;
			position.x = position.y = position.z = FLT_MAX;
		}
	}
}