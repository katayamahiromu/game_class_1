#pragma once
#include"Graphics/Model.h"
#include"Enemy.h"
#include"Effect.h"
#include"Audio/Audio.h"

//�X���C��
class EnemyShell :public Enemy
{
public:
	EnemyShell();
	~EnemyShell()override;
	
	//�X�V����
	void Update(float& elapsedTime)override;

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//�f�o�b�N�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

	//�꒣��ݒ�
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);
	std::unique_ptr<Effect>hit[5];
protected:
	//���S�������ɌĂ΂��
	void OnDead()override;
	//�_���[�W���󂯂����ɌĂ΂��
	void OnDamaged() override;
private:
	void UpdateDeadState(float elapsedTime);
	void TransitionDeadState();
	void ResPornTransition();
private:
	enum class State
	{
		Normal,
		Dead,
		Damage,
		ResPorn,
	};

	State state = State::Normal;
	DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
	DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };
	float territoryRange = 10.0f;
	float moveSpeed = 3.0f;
	float turnSpeed = DirectX::XMConvertToRadians(360);
	float stateTimer = 0.0f;
	float searchRange = 5.0f;
	float attackRange = 1.5f;

	//HP���T�̂��߂T�p��
	std::unique_ptr<Effect>damage_effect[5];
	std::unique_ptr<AudioSource>damage_se[5];
	//�c�����H���{�����\�b�h
	std::unique_ptr<SubMixVoice> reverberation;
};
