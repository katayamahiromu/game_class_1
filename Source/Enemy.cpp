#include"Enemy.h"
#include"Graphics/Graphics.h"
#include"EnemyManeger.h"
#include"Player.h"

//�f�o�b�O�v���~�e�B�u�`��
void Enemy::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	////�Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//�Փ˗p����p�̃f�o�b�O�~����`��
	DirectX::XMFLOAT3 pos = position;
	pos.y += 5.0f;
	float rad = radius + 5.0f;
	float hei = height + 5.0f;
	debugRenderer->DrawCylinder(pos, rad, hei, DirectX::XMFLOAT4(0, 0, 0, 1));
	pos.x += 5.0;
	debugRenderer->DrawCylinder(pos, rad, hei, DirectX::XMFLOAT4(0, 0, 0, 1));
	//debugRenderer->DrawCylinder(pos, rad, hei, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//�j��
void Enemy::Destroy() {
	EnemeyManager::Instance().Remove(this);
}