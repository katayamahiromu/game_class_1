#pragma once

#include"Graphics/Shader.h"
#include"Graphics/Sprite.h"
#include"Character.h"
#include"EnemyManeger.h"
#include"Collision.h"
#include"ProjectileManager.h"
#include"Effect.h"
#include<list>
#include"Audio/Audio.h"

//プレイヤー
class Player :public Character
{
public:
	Player();
	~Player() override;

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//デバッグ用GUI描画
	void DrawDebugGui();
	//スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;
	//移動入力処理
	bool InputMove(float elapsedTime);
	//デバッグプリミティブ描画
	void DrawDebugPrimitive();
	//プレイヤーとエネミーとの衝突判定
	void CollisionPlayerVsEnemies();
	//プレイヤーと精子の衝突判定
	void CollisionPlayerVsSperm();
	//ジャンプ入力処理
	bool InputJump();
	//着地した時に呼ばれる
	void OnLanding()override;
	//弾丸入力処理
	//void InputProjectile();
	//弾丸と敵の衝突判定
	void CollisionProjectilesVsEnemies();
	//攻撃入力処理
	bool InputAttack();
	//ノードとエネミーの衝突処理
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);
	//インスタンス取得
	static Player& Instance();

	//ターゲットを探してロックオンする処理
	Enemy*Target();
	void Input_Target();
	Enemy* Get_Target_Enemy() { return target_enemy; }
	void Set_target_enemy() { target_enemy = nullptr; }
	int Get_Rock_num() { return Rock_num; }
	int kill_awabi = 0;
	float timer = 180.0f;
	DirectX::XMFLOAT3 GetVelocity() { return velocity; }
protected:
	//ダメージを受けた時に呼ばれる
	void OnDamaged() override;
	//死亡した時に呼ばれる
	void OnDead()override;
private:
	//待機ステートへ遷移
	void TransitiomIdleState();

	//待機ステート更新処理
	void UpdateIdelState(float elapsedTime);

	//移動ステートへ遷移
	void TransitionMoveState();

	//移動ステート更新処理
	void UpdateMoveState(float elapseTime);

	//ジャンプステートへ遷移
	void TransitionJumpState();

	//ジャンプステート更新処理
	void UpdateJumpState(float elapsedTime);

	//着地ステートへ遷移
	void TransitionLandState();

	//着地ステート更新処理
	void UpdateLandState(float elapsedTime);

	//攻撃ステートへ遷移
	void TransitionAttackState();

	//攻撃ステート更新処理
	void UpdateAttackState(float elapsedTime);

	//ダメージステートへ遷移
	void TransitionDamageState();

	//ダメージステート更新処理
	void UpdateDamageState(float elapsedTime);

	//死亡ステートへ遷移
	void TransitionDeathState();

	//死亡ステート更新処理
	void UpdateDeathState(float elapsedTime);

	//復活ステートへ遷移
	void TransitionReviveState();

	//復活ステート更新処理
	void UpdateReviveState(float elapsedTime);

	void InputVerticalMove(float elapsedTime);

private:
	float moveSpeed = 10.0f;
	float turnSpeed = DirectX::XMConvertToRadians(720);
	float JumpSpeed = 20.0f;
	float updownSpeed = 3.0f;
	float diagonalSpeed = 2.0f;
	int jumpCount = 0;
	int jumpLimit = 2;
	ProjectileManager projectileManager;
	Effect* hitEffect = nullptr;
	float leftHandRadius = 0.4f;
	bool attackCollisionFlag = false;
	//アニメーション
	enum Animation
	{
		Anime_Idle,
		Anime_Attack,
		Anime_Death,
		Anime_Falling,
		Anime_GetHit1,
		Anime_GetHit2,
		Anime_Jump,
		Anime_Jump_Flip,
		Anime_Landing,
		Anime_Revive,
		Anime_Running,
		Anime_Walking
	};

	//ステート
	enum class State
	{
		Idle,
		Move,
		Jump,
		Land,
		Attack,
		Damage,
		Death,
		Revive
	};
private:
	State state = State::Idle;
private:
	float target_range = 30.0f;
	Enemy* target_enemy;
	int Rock_num;//敵のインデックス番号
	//攻撃可能な精子のインデックス番号を保存する
	//FIFO形式でデータを取り出すならVectorよりListのほうが多分はやい？
	std::list<int>can_attack_sperm;
	std::unique_ptr<AudioSource>move_se;
	std::unique_ptr<AudioSource>catch_se;
};