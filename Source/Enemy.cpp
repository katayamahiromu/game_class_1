#include"Enemy.h"
#include"Graphics/Graphics.h"
#include"EnemyManeger.h"
#include"Player.h"

//デバッグプリミティブ描画
void Enemy::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	////衝突判定用のデバッグ球を描画
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//衝突用判定用のデバッグ円柱を描画
	DirectX::XMFLOAT3 pos = position;
	pos.y += 5.0f;
	float rad = radius + 5.0f;
	float hei = height + 5.0f;
	debugRenderer->DrawCylinder(pos, rad, hei, DirectX::XMFLOAT4(0, 0, 0, 1));
	pos.x += 5.0;
	debugRenderer->DrawCylinder(pos, rad, hei, DirectX::XMFLOAT4(0, 0, 0, 1));
	//debugRenderer->DrawCylinder(pos, rad, hei, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//破棄
void Enemy::Destroy() {
	EnemeyManager::Instance().Remove(this);
}