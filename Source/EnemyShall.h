#pragma once
#include"Graphics/Model.h"
#include"Enemy.h"
#include"Effect.h"
#include"Audio/Audio.h"

//スライム
class EnemyShell :public Enemy
{
public:
	EnemyShell();
	~EnemyShell()override;
	
	//更新処理
	void Update(float& elapsedTime)override;

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//デバックプリミティブ描画
	void DrawDebugPrimitive() override;

	//縄張り設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);
	std::unique_ptr<Effect>hit[5];
protected:
	//死亡した時に呼ばれる
	void OnDead()override;
	//ダメージを受けた時に呼ばれる
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

	//HPが５のため５つ用意
	std::unique_ptr<Effect>damage_effect[5];
	std::unique_ptr<AudioSource>damage_se[5];
	//残響加工を施すメソッド
	std::unique_ptr<SubMixVoice> reverberation;
};
