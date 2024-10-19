#include"EnemyShall.h"
#include"Graphics/Graphics.h"
#include"Mathf.h"
#include"Player.h"

//コンストラクタ
EnemyShell::EnemyShell() {
	mdl = std::make_unique<Model>("Data/Model/Shell/shell.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.5f;
	radius = 2.3f;
	height = 1.0f;
	mdl->PlayAnimation(0, true, 0.2f);

	health = 5.0f;
}

//デストラクタ
EnemyShell::~EnemyShell() {
}

//更新処理
void EnemyShell::Update(float& elapsedTime)
{
	//速力更新処理
	UpdateVelocity(elapsedTime);
	//無敵時間の更新
	UpdateInvinciblTImer(elapsedTime);
	//オブジェクト行列を更新
	UpdateTranceform();

	//モデルアニメーション更新
	mdl->UpdateAnimation(elapsedTime);

	mdl->UpdateTransform(transform);

	//死んだ時の回転
	//angle.x = Mathf::Leap(angle.x,DirectX::XMConvertToRadians(180), elapsedTime);
}

//描画処理
void EnemyShell::Render(ID3D11DeviceContext* dc, Shader* shader) 
{
	mdl->mask.dissolveThreshold = mask.dissolveThreshold;
	mdl->mask.edgColor = mask.edgColor;
	mdl->mask.edgThreshold = mask.edgThreshold;
	shader->Draw(dc, mdl.get());
}

//ダメージを受けた時に呼ばれる
void EnemyShell::OnDamaged()
{
	//ダメージステートへ遷移
	//TransitionDamageState();
}

//死亡した時に呼ばれる
void EnemyShell::OnDead()
{
	//死亡ステートへ遷移
	//TransitionDeathState();
}

//デバックプリミティブ描画
void EnemyShell::DrawDebugPrimitive()
{
	//基底クラスのデバックプリミティブ描画
	//Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	DirectX::XMFLOAT3 pos = position;
	pos.y += 1.0f;
	debugRenderer->DrawSphere(pos, radius, DirectX::XMFLOAT4(1, 0, 0, 1));
}

//縄張り設定
void EnemyShell::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}