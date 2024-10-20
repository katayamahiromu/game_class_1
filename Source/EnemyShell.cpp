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

	damage_effect[0] = std::make_unique<Effect>("Data/Effect/HIT/edit_maternity.efk");
	damage_effect[1] = std::make_unique<Effect>("Data/Effect/HIT/edit_crezy.efk");
	damage_effect[2] = std::make_unique<Effect>("Data/Effect/HIT/edit_excite.efk");
	damage_effect[3] = std::make_unique<Effect>("Data/Effect/HIT/edit_great.efk");
	damage_effect[4] = std::make_unique<Effect>("Data/Effect/HIT/edit_hit.efk");

	damage_se[0] = Audio::Instance().LoadAudioSource("Data/Audio/マタニティ.wav");
	damage_se[1] = Audio::Instance().LoadAudioSource("Data/Audio/クレイジー.wav");
	damage_se[2] = Audio::Instance().LoadAudioSource("Data/Audio/エキサイティン.wav");
	damage_se[3] = Audio::Instance().LoadAudioSource("Data/Audio/グレート.wav");
	damage_se[4] = Audio::Instance().LoadAudioSource("Data/Audio/ヒット.wav");

	reverberation = Audio::Instance().MakeSubMix();
	for (int i = 0;i < 5;i++)
	{
		damage_se[i]->Set_Submix_voice(reverberation->Get_Submix_Voice());
	}
	reverberation->Reverb();
}

//デストラクタ
EnemyShell::~EnemyShell() {
}

//更新処理
void EnemyShell::Update(float& elapsedTime)
{
	switch (state)
	{
	case EnemyShell::State::Normal:
		break;
	case EnemyShell::State::Dead:
		UpdateDeadState(elapsedTime);
		break;
	case EnemyShell::State::Damage:
		break;
	default:
		break;
	}
	//速力更新処理
	UpdateVelocity(elapsedTime);
	//無敵時間の更新
	UpdateInvinciblTImer(elapsedTime);
	//オブジェクト行列を更新
	UpdateTranceform();

	//モデルアニメーション更新
	mdl->UpdateAnimation(elapsedTime);

	mdl->UpdateTransform(transform);
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
	DirectX::XMFLOAT3 pos = position;
	pos.y += 2.0f;
	damage_effect[static_cast<int>(health)]->Play(pos);
	damage_se[static_cast<int>(health)]->Play(false);
	//ダメージステートへ遷移
	//TransitionDamageState();
}

//死亡した時に呼ばれる
void EnemyShell::OnDead()
{
	DirectX::XMFLOAT3 pos = position;
	pos.y += 2.0f;
	damage_effect[static_cast<int>(health)]->Play(pos);
	damage_se[static_cast<int>(health)]->Play(false);
	TransitionDeadState();
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

void EnemyShell::TransitionDeadState()
{
	state = State::Dead;
}

void EnemyShell::UpdateDeadState(float elapsedTime)
{
	mask.dissolveThreshold = Mathf::Leap(mask.dissolveThreshold, 0.0f, elapsedTime);
	//死んだ時の回転
	angle.x = Mathf::Leap(angle.x,DirectX::XMConvertToRadians(180), elapsedTime);
	Player::Instance().Set_target_enemy();
	if (mask.dissolveThreshold <= 0.1f)
	{
		ResPornTransition();
		Player::Instance().kill_awabi++;
	}
}

void EnemyShell::ResPornTransition()
{
	state = State::Normal;
	targetPosition = Player::Instance().GetPosition();

	targetPosition.x += (rand() % 30) - 15;
	targetPosition.y += (rand() % 30) - 15;
	targetPosition.z += (rand() % 30) - 15;
	
	position = targetPosition;
	mask.dissolveThreshold = 1.0;
	health = 5.0;
	angle.x = DirectX::XMConvertToRadians(0);
}