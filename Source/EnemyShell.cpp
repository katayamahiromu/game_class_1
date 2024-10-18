#include"EnemyShall.h"
#include"Graphics/Graphics.h"
#include"Mathf.h"
#include"Player.h"

//�R���X�g���N�^
EnemyShell::EnemyShell() {
	mdl = std::make_unique<Model>("Data/Model/Shell/shell.mdl");

	//���f�����傫���̂ŃX�P�[�����O
	scale.x = scale.y = scale.z = 0.5f;
	radius = 2.3f;
	height = 1.0f;
	mdl->PlayAnimation(0, true, 0.2f);

	health = 5.0f;
}

//�f�X�g���N�^
EnemyShell::~EnemyShell() {
}

//�X�V����
void EnemyShell::Update(float& elapsedTime)
{
	//���͍X�V����
	UpdateVelocity(elapsedTime);
	//���G���Ԃ̍X�V
	UpdateInvinciblTImer(elapsedTime);
	//�I�u�W�F�N�g�s����X�V
	UpdateTranceform();

	//���f���A�j���[�V�����X�V
	mdl->UpdateAnimation(elapsedTime);

	mdl->UpdateTransform(transform);

	//���񂾎��̉�]
	//angle.x = Mathf::Leap(angle.x,DirectX::XMConvertToRadians(180), elapsedTime);
}

//�`�揈��
void EnemyShell::Render(ID3D11DeviceContext* dc, Shader* shader) 
{
	mdl->mask.dissolveThreshold = mask.dissolveThreshold;
	mdl->mask.edgColor = mask.edgColor;
	mdl->mask.edgThreshold = mask.edgThreshold;
	shader->Draw(dc, mdl.get());
}

//�_���[�W���󂯂����ɌĂ΂��
void EnemyShell::OnDamaged()
{
	//�_���[�W�X�e�[�g�֑J��
	//TransitionDamageState();
}

//���S�������ɌĂ΂��
void EnemyShell::OnDead()
{
	//���S�X�e�[�g�֑J��
	//TransitionDeathState();
}

//�f�o�b�N�v���~�e�B�u�`��
void EnemyShell::DrawDebugPrimitive()
{
	//���N���X�̃f�o�b�N�v���~�e�B�u�`��
	//Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	DirectX::XMFLOAT3 pos = position;
	pos.y += 1.0f;
	debugRenderer->DrawSphere(pos, radius, DirectX::XMFLOAT4(1, 0, 0, 1));
}

//�꒣��ݒ�
void EnemyShell::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}