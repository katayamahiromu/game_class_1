#pragma once
#include"Character.h"
#include"Graphics/Model.h"
#include"Graphics/Shader.h"

//自機と同じ名前のためチャイルド
class Sperm_child :public Character
{
public:
	Sperm_child();
	~Sperm_child();

	void Update(float& elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);
	void SetRandomTargetPosition();
	void Set_player_catch(bool pcatch) { player_catch = pcatch; }
private:
	void MoveToTarget(const float& elapsedTime, float speedRate);
private:
	std::unique_ptr<Model>mdl;

	DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
	DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };
	float territoryRange = 10.0f;
	float moveSpeed = 3.0f;
	float turnSpeed = DirectX::XMConvertToRadians(360);
private:
	enum class State
	{
		Wander,
		Idle,
		Follow,
	};

	//徘徊ステートへ遷移
	void TransitionWanderState();

	//徘徊ステート更新処理
	void UpdateWanderState(float elaspedTime);

	//待機ステートへ
	void TransitionIdleState();

	//待機ステート更新処理
	void UpdateIdleState(float elapsedTime);

	//追従ステートへ遷移
	void TransitionFollowState();
	//追従ステート更新
	void UpdateFollowState(float elapedTime);

	State state = State::Wander;
	float stateTimer = 0.0f;
private:
	//プレイヤーに捕まったかどうか
	bool player_catch = false;
	float stopRange = 1.5f;
};