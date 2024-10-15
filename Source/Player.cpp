#include<imgui.h>
#include"Player.h"
#include"Input/Input.h"
#include"Camera.h"
#include"Graphics/Graphics.h"
#include"ProjectileStraight.h"
#include"ProjectileHoming.h"
#include"EnemyManeger.h"
#include"Sperm_Manager.h"
#include"Mathf.h"


static Player* instace = nullptr;

//インスタンス取得
Player& Player::Instance()
{
	return *instace;
}

//コンストラクタ
Player::Player() {
	//インスタンスポインタ取得
	instace = this;

	model = std::make_unique<Model>("Data/Model/Player/player.mdl");
	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.1f;

	//ヒットエフェクト読み込み
	hitEffect = new Effect("Data/Effect/Hit.efk");

	//待機ステートへの遷移
	TransitiomIdleState();
}

//デストラクタ
Player::~Player() {
	delete hitEffect;
}

//更新処理
void Player::Update(float elapsedTime) {
	//ステート毎の処理
	switch (state)
	{
	case State::Idle:
		UpdateIdelState(elapsedTime);
		break;
	case State::Move:
		UpdateMoveState(elapsedTime);
		break;
	case State::Jump:
		UpdateJumpState(elapsedTime);
		break;
	case State::Land:
		UpdateLandState(elapsedTime);
		break;
	case State::Attack:
		UpdateAttackState(elapsedTime);
		break;
	case State::Damage:
		UpdateDamageState(elapsedTime);
		break;
	case State::Death:
		UpdateDeathState(elapsedTime);
		break;
	case State::Revive:
		UpdateReviveState(elapsedTime);
		break;
	}

	//速力更新
	UpdateVelocity(elapsedTime);
	//無敵時間更新
	UpdateInvinciblTImer(elapsedTime);

	InputVerticalMove(elapsedTime);

	CollisionPlayerVsSperm();
	//プレイヤーと敵との衝突判定
	CollisionPlayerVsEnemies();
	//弾丸と敵の衝突判定
	CollisionProjectilesVsEnemies();
	//ターゲット用入力受付
	Input_Target();
	//オブジェクト行列を更新
	UpdateTranceform();
	//弾丸発射処理
	projectileManager.Update(elapsedTime);
	//モデルアニメーションを更新
	model->UpdateAnimation(elapsedTime);
	//モデル行列を更新
	model->UpdateTransform(transform);
}

//移動入力処理
bool Player::InputMove(float elapsedTime) {
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//移動処理
	Move(moveVec.x, moveVec.z, moveSpeed);
	//旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	return (moveVec.x != 0.0f || moveVec.z != 0.0f) ? true : false;
}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader) {

	shader->Draw(dc, model.get());
	//弾丸描画処理
	projectileManager.Render(dc, shader);
}

void Player::DrawDebugGui() {
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Player", nullptr, ImGuiTreeNodeFlags_DefaultOpen)) {
		//トランスフォーム
		if (ImGui::CollapsingHeader("Tranceform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//位置
			ImGui::InputFloat3("Position", &position.x);
			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//スケール
			ImGui::InputFloat3("Scale", &scale.x);

			ImGui::InputFloat("Target Range", &target_range);

			int i = can_attack_sperm.size();
			ImGui::InputInt("Can Attack list Count", &i);
		}
	}
	ImGui::End();
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//入力情報を取得 
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	//カメラ方向とスティックの入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//移動ベクトルはXZ平面に水平なベクトルになるようにする

	//カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

	if(cameraRightLength > 0.0f)
	{
		//単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	//カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);

	if (cameraFrontLength > 0.0f)
	{
		//単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	//ステックの水平入力値をカメラ右方向に反映し、
	//ステックの垂直入力値をカメラ前方向に反映し、
	//進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = cameraFrontX * ay + cameraRightX * ax;
	vec.z = cameraFrontZ * ay + cameraRightZ * ax;
	//Y方向は移動しない
	vec.y = 0.0f;

	return vec;
}

void Player::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	////衝突判定用のデバッグ球を描画
	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//ターゲット用
	debugRenderer->DrawSphere(position, target_range, DirectX::XMFLOAT4(1, 0, 0, 1));
	//衝突用判定用のデバッグ円柱を描画
	//debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	//projectileManager.DrawDebugPrimitive();

	//攻撃衝突用の左手ノードのデバッグ球を描画
	if (attackCollisionFlag) {
		Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
		debugRenderer->DrawSphere(DirectX::XMFLOAT3(
			leftHandBone->worldTransform._41,
			leftHandBone->worldTransform._42,
			leftHandBone->worldTransform._43),
			leftHandRadius,
			DirectX::XMFLOAT4(1, 0, 0, 1)
		);
	}
}

void Player::CollisionPlayerVsEnemies() {
	EnemeyManager& enemyManager = EnemeyManager::Instance();
	//全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0;i < enemyCount;++i) {
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMFLOAT3 outPosition;

		//衝突判定
		if (Collision::IntersectCylinderVsCylinder(
			position,
			radius,
			height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition
		)) {
			float closs = (position.y * enemy->GetPosition().z) - (position.z * enemy->GetPosition().y);
			if (closs > 0.0f) {
				enemy->ApplyDamage(1,0.5f);
				Junp(JumpSpeed * 0.5);
			}
			else {
				enemy->SetPositon(outPosition);
			}
		}
	};
}

//ジャンプ入力処理
bool Player::InputJump() {
	/*GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_SPACE) {
		jumpCount++;
		if(jumpCount < jumpLimit) Junp(JumpSpeed);
		if (isGround) {
			jumpCount = 0;
			return true;
		}
	}*/
	return false;
}

void Player::OnLanding() {
	isGround = true;

	//下方向の速力が一定以上なら着地ステートへ
	if (velocity.y < 1.0f) {
		//死亡、ダメージステート時は着地ステート遷移しないようにする
		if (state != State::Damage && state != State::Death) {
			TransitionLandState();
		}
	}
}

//ダメージを受けた時に呼ばれる
void Player::OnDamaged()
{
	//ダメージステートへ遷移
	TransitionDamageState();
}

//死亡した時に呼ばれる
void Player::OnDead()
{
	//死亡ステートへ遷移
	TransitionDeathState();
}

//弾丸入力処理
void Player::InputProjectile() 
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//直進弾丸発射
	if (gamePad.GetButtonDown() & GamePad::BTN_X)
	{
		//前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0;
		dir.z = cosf(angle.y);
		//発射位置（プレイヤーの腰あたり）
		DirectX::XMFLOAT3 pos;
		pos.x = GetPosition().x;
		pos.y = GetPosition().y + 1;
		pos.z = GetPosition().z;

		//発射
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
	}

	//ぐるぐるは仕様　自分で改造しよう！
	if (gamePad.GetButtonDown() & GamePad::BTN_Y)
	{
		//前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0;
		dir.z = cosf(angle.y);
		//発射位置（プレイヤーの腰あたり）
		DirectX::XMFLOAT3 pos;
		pos.x = GetPosition().x;
		pos.y = GetPosition().y + 1;
		pos.z = GetPosition().z;

		//ターゲット（デフォルトではプレイヤーの前方）
		DirectX::XMFLOAT3 target;
		target.x = dir.x;
		target.y = dir.y;
		target.z = dir.z;

		//一番近くの敵をターゲットにする
		float dist = FLT_MAX;
		EnemeyManager& enemyManager = EnemeyManager::Instance();
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0;i < enemyCount;i++)
		{
			//敵との距離判定
			Enemy* enemy = EnemeyManager::Instance().GetEnemy(i);
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
			float d;
			DirectX::XMStoreFloat(&d, D);
			if (d < dist) {
				dist = d;
				target = enemy->GetPosition();
				target.y += enemy->GetHeight() * 0.5f;
			}
		}

		//発射
		ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
		projectile->Launch(dir, pos,target);
	}
}

//弾丸と敵の衝突判定
void Player::CollisionProjectilesVsEnemies(){
	EnemeyManager& enemyManager = EnemeyManager::Instance();
	//全ての弾丸と全ての敵を総当たりで衝突判定
	int projectileCount = projectileManager.GetProjectileCOunt();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0;i < projectileCount;++i) {
		Projectile* projectile = projectileManager.GetProjectile(i);
		for (int j = 0;j < enemyCount;++j) {
			Enemy* enemy = enemyManager.GetEnemy(j);
			//衝突処理
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersctSphereVsCylinder(
				projectile->GetPosition(),
				projectile->GetRadius(),
				enemy->GetPosition(),
				enemy->GetRadius(),
				enemy->GetHeight(),
				outPosition)
				)
			{
				//ダメージを与える
				if (enemy->ApplyDamage(1, 0.5f))
				{
					//吹き飛ばす
					DirectX::XMFLOAT3 impulse;
					impulse.x = sinf(angle.y) * 10.0f;
					impulse.y = 0.0f;
					impulse.z = cosf(angle.y) * 10.0f;

					////別解
					//{
					//	DirectX::XMFLOAT3 Impulse;
					//	const float power = 10.0f;
					//	//敵の位置を取得
					//	const DirectX::XMFLOAT3& e = enemy->GetPosition();
					//	//弾の位置を取得
					//	const DirectX::XMFLOAT3& p = projectile->GetPosition();
					//	//弾から敵の位置へのベクトルを取得
					//	float vx = p.x - e.x;
					//	float vz = p.z - e.z;
					//	//そのベクトル正規化
					//	float lengthXZ = sqrtf(vx * vx + vz * vz);
					//	vx /= lengthXZ;
					//	vz /= lengthXZ;
					//	//衝撃の値を設定（xzは正規化したベクトルをpower分スケーリング）
					//	Impulse.x = vx * power;
					//	Impulse.y = power * 0.5f;//ｙはちょっと浮かせる
					//	Impulse.z = vz * power;
					//	
					//}
					enemy->AddImpulse(impulse);

					//ヒットエフェクト再生
					DirectX::XMFLOAT3 e = enemy->GetPosition();
					e.y += enemy->GetHeight() * 0.5f;
					hitEffect->Play(e);

					//弾丸破棄
					projectile->Destroy();
				}
			}
		}
	}
}

void Player::CollisionPlayerVsSperm()
{
	Sperm_Manager& sperm_manager = Sperm_Manager::Instance();
	int Sperm_count = sperm_manager.GetSpermCount();
	for (int i = 0;i < Sperm_count;i++)
	{
		if (sperm_manager.GetSperm(i)->Get_player_catch())continue;
		Sperm_child* sperm = sperm_manager.GetSperm(i);
		if (Collision::IntersectSphereVsSphere(
			position,
			radius,
			sperm->GetPosition(),
			sperm->GetRadius(),
			DirectX::XMFLOAT3(0,0,0)
		))
		{
			sperm_manager.GetSperm(i)->Set_player_catch(true);
			can_attack_sperm.push_back(i);
		}
	}
}

bool Player::InputAttack()
{
	//攻撃出来る精子がいない場合クラッシュする
	if (can_attack_sperm.size() == 0)return false;
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		Sperm_Manager& sperm_manager = Sperm_Manager::Instance();
		//listの先頭データを入手
		sperm_manager.GetSperm(can_attack_sperm.front())->Change_Attack_Modo();
		//攻撃命令を出した後は削除
		can_attack_sperm.pop_front();
		return true;
	}
	return false;
}

//待機ステートへ遷移
void Player::TransitiomIdleState()
{
	state = State::Idle;

	//待機アニメーション再生
	model->PlayAnimation(Anime_Idle, true,0.2f);
}

//待機ステート更新処理
void Player::UpdateIdelState(float elapsedTime)
{
	//移動入力処理
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
	}
	
	//ジャンプ入力処理
	if (InputJump())
	{
		TransitionJumpState();
	}

	//攻撃ステート
	if (InputAttack()) {
		TransitionAttackState();
	}

	//弾丸入力処理
	InputProjectile();
}

//移動ステートへの遷移
void Player::TransitionMoveState()
{
	state = State::Move;

	//走りアニメーションの再生
	model->PlayAnimation(Anime_Running, true,0.2f);
}

//移動ステート更新処理
void Player::UpdateMoveState(float elapsedTime)
{
	//移動入力処理
	if (!InputMove(elapsedTime))
	{
		TransitiomIdleState();
	}
	//ジャンプ入力処理
	if (InputJump())
	{
		TransitionJumpState();
	}

	//攻撃ステート更新処理
	if (InputAttack()) {
		TransitionAttackState();
	}

	//弾丸入力処理
	InputProjectile();
}

//ジャンプステートへ遷移
void Player::TransitionJumpState()
{
	state = State::Jump;

	//ジャンプアニメーション再生
	model->PlayAnimation(Anime_Jump, false, 0.2f);
}

//ジャンプステート更新処理
void Player::UpdateJumpState(float elapsedTime)
{
	//空中移動？
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
	}

	if (InputJump())
	{
		TransitionJumpState();
	}

	InputProjectile();
}

//着地ステートへ遷移
void Player::TransitionLandState()
{
	state = State::Land;

	//着地アニメーション再生
	model->PlayAnimation(Anime_Landing, false, 0.2f);
}

//着地ステート更新処理
void Player::UpdateLandState(float elapsedTime)
{
	if (InputMove(elapsedTime))
	{
		TransitionMoveState();
	}
	else 
	{
		TransitiomIdleState();
	}


	if (InputJump())
	{
		TransitionJumpState();
	}
}

//攻撃ステートへの遷移
void Player::TransitionAttackState()
{
	state = State::Attack;
	model->PlayAnimation(Anime_Attack, false, 0.2f);
}

//攻撃ステート更新処理
void Player::UpdateAttackState(float elapsedTime)
{
	if (!model->IsPlayAnimation()) {
		TransitiomIdleState();
	}
	//任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentAnimationSeconds();
	attackCollisionFlag = (animationTime > 0.3 && animationTime < 0.4) ? true : false;
	if (attackCollisionFlag) {
		//左手ノードとエネミーの衝突判定
		CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
	}
}

//ノードと敵の衝突処理
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
	//ノード取得
	Model::Node* node = model->FindNode(nodeName);

	//ノード位置取得
	DirectX::XMFLOAT3 nodePosition;
	nodePosition.x = node->worldTransform._41;
	nodePosition.y = node->worldTransform._42;
	nodePosition.z = node->worldTransform._43;

	//指定のノードと全ての敵を総当たりで衝突判定
	EnemeyManager& enemyManager = EnemeyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0;i < enemyCount;++i) {
		Enemy* enemy = enemyManager.GetEnemy(i);

		//衝突判定
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersctSphereVsCylinder(
			nodePosition,
			nodeRadius,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition)
			)
		{
			enemy->SetPositon(outPosition);
		}
	}
}

//ダメージステートへの遷移
void Player::TransitionDamageState()
{
	state = State::Damage;
	
	//ダメージアニメーション再生
	model->PlayAnimation(Anime_GetHit1, false,0.2f);
}

//ダメージステート更新処理
void Player::UpdateDamageState(float elapsedTime)
{
	//ダメージアニメーションが終わったら待機ステートへ
	if (!model->IsPlayAnimation())
	{
		TransitiomIdleState();
	}
}

//死亡ステートへ遷移
void Player::TransitionDeathState()
{
	state = State::Death;
	//死亡アニメーション再生
	model->PlayAnimation(Anime_Death, false, 0.2f);
}

//死亡ステート更新処理
void Player::UpdateDeathState(float elapsedTime)
{
	if (!model->IsPlayAnimation())
	{
		//ボタンを押したら復活ステートへ遷移
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_A)
		{
			TransitionReviveState();
		}
	}
}

//復活ステートへ遷移
void Player::TransitionReviveState()
{
	state = State::Revive;
	//体力回復
	health = maxHealth;

	//復活アニメーション再生
	model->PlayAnimation(Anime_Revive, false, 0.2f);
}

//復活ステート更新処理
void Player::UpdateReviveState(float elapsedTime)
{
	//復活アニメーション終了後に待機ステートへ
	if (!model->IsPlayAnimation())
	{
		TransitiomIdleState();
	}
}

Enemy* Player::Target()
{
	float dis = FLT_MAX;
	Enemy* near_enemy = nullptr;
	int Enemy_Count = EnemeyManager::Instance().GetEnemyCount();
	Rock_num = NULL;
	for (int i = 0;i < Enemy_Count;i++)
	{
		Enemy* e = EnemeyManager::Instance().GetEnemy(i);
		DirectX::XMVECTOR Distance = DirectX::XMVectorSubtract(
			DirectX::XMLoadFloat3(&position), 
			DirectX::XMLoadFloat3(&e->GetPosition())
		);
		
		float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(Distance));
		if (Collision::IntersectSphereVsSphere(
			position,
			target_range,
			e->GetPosition(),
			e->GetRadius(),
			DirectX::XMFLOAT3(0,0,0)
		))
		{
			if (dis > distance)
			{
				dis = distance;
				near_enemy = e;
				Rock_num = i;
			}
		}
	}
	return near_enemy;
}

void Player::Input_Target()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_ENTER)target_enemy = Target();
}


//垂直方向入力
void Player::InputVerticalMove(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();
	if (gamePad.GetButton() & GamePad::BTN_SPACE)
	{
		float diagonalSpeed = maxMoveSpeed;
		
		/*velocity.y =ay * updownSpeed+ax*0.0f;	
		angle.x = DirectX::XMConvertToRadians(-90.0f);*/
		if (GetAsyncKeyState('W') & 0x8000)
		{
			velocity.y = updownSpeed;
			velocity.x = 0.0f;
			velocity.z = 0.0f;
			angle.x = Mathf::Leap(angle.x, DirectX::XMConvertToRadians(-90.0f), elapsedTime*10);
			if (GetAsyncKeyState('A') & 0x8000)
			{
				velocity.x -=diagonalSpeed;
			}
			if (GetAsyncKeyState('D') & 0x8000)
			{
				velocity.x += diagonalSpeed;
			}

		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{

			velocity.y = -updownSpeed;
			velocity.x = 0.0f;
			velocity.z = 0.0f;
			angle.x = Mathf::Leap(angle.x, DirectX::XMConvertToRadians(90.0f), elapsedTime*10);

			if (GetAsyncKeyState('A') & 0x8000)
			{
				velocity.x -= diagonalSpeed;
			}
			if (GetAsyncKeyState('D') & 0x8000)
			{
				velocity.x += diagonalSpeed;
			}
		}
	}
	else
	{
		velocity.y = 0.0f;
	}

}

