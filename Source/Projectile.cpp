#include"Projectile.h"
#include"ProjectileManager.h"
#include"Graphics/Graphics.h"

//コンストラクタ
Projectile::Projectile(ProjectileManager* manager):manager(manager)
{
	manager->Register(this);
}


//デバックプリミティブ描画
void Projectile::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	debugRenderer->DrawSphere(this->GetPosition(), radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}
//行列更新処理
void Projectile::UpdateTransform() {
	DirectX::XMVECTOR Front, Up, Right;
	
	//前ベクトルを算出
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);
	//仮の上ベクトルを算出
	Up = { 0,1,0,0 };
	//右ベクトルを算出
	Right = DirectX::XMVector3Cross(Front, Up);
	Right = DirectX::XMVector3Normalize(Right);
	//上ベクトルを算出
	Up = DirectX::XMVector3Cross(Right, Front);
	Up = DirectX::XMVector3Normalize(Up);

	//計算結果を取り出し
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	//算出した軸ベクトルから行列を作成
	transform._11 = scale.x * right.x;
	transform._12 = scale.x * right.y;
	transform._13 = scale.x * right.z;
	transform._14 = 0.0f;
	transform._21 = scale.y * up.x;
	transform._22 = scale.y * up.y;
	transform._23 = scale.y * up.z;
	transform._24 = 0.0f;
	transform._31 = scale.z * front.x;
	transform._32 = scale.z * front.y;
	transform._33 = scale.z * front.z;
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;

	//発射方向
	this->direction = front;
}

//破棄
void Projectile::Destroy() {
	manager->Remove(this);
}

//反射
void Projectile::reflection()
{

}