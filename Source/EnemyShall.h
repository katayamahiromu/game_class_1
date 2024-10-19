#pragma once
#include"Graphics/Model.h"
#include"Enemy.h"

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
protected:
	//���S�������ɌĂ΂��
	void OnDead()override;
	//�_���[�W���󂯂����ɌĂ΂��
	void OnDamaged() override;
private:
	enum class State
	{
		Normal,
		Dead,
		Damage
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
};
