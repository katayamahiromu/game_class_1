#pragma once
#include"Character.h"
#include"Graphics/Shader.h"

//���@�Ɠ������O�̂��߃`���C���h
class Sperm_child :public Character
{
public:
	Sperm_child();
	~Sperm_child();

	void Update(float& elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);
	void SetRandomTargetPosition();
	void Set_player_catch(bool pcatch) { player_catch = pcatch; }
	bool Get_player_catch() { return player_catch; }
	//�Ă΂�邱�Ƃ�
	void Change_Attack_Modo() { TransitionAttackState(); }
private:
	void MoveToTarget(const float& elapsedTime, float speedRate);
private:
	DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
	DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };
	float territoryRange = 10.0f;
	float moveSpeed = 3.0f;
	float turnSpeed = DirectX::XMConvertToRadians(360);
private:
	enum class State
	{
		Wander,
		Idle,
		Follow,
		Attack,
	};

	//�p�j�X�e�[�g�֑J��
	void TransitionWanderState();

	//�p�j�X�e�[�g�X�V����
	void UpdateWanderState(float elaspedTime);

	//�ҋ@�X�e�[�g��
	void TransitionIdleState();

	//�ҋ@�X�e�[�g�X�V����
	void UpdateIdleState(float elapsedTime);

	//�Ǐ]�X�e�[�g�֑J��
	void TransitionFollowState();
	//�Ǐ]�X�e�[�g�X�V
	void UpdateFollowState(float elapedTime);

	//�U���X�e�[�g�֑J��
	void TransitionAttackState();
	//�R�X�v���ٔ��X�V
	void UpdateAttack(float elapsedTime);

	State state = State::Wander;
	float stateTimer = 0.0f;
private:
	//�v���C���[�ɕ߂܂������ǂ���
	bool player_catch = false;
	bool attack = false;
	float stopRange = 1.5f;
	bool isActive = true;
};