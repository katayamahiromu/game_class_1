#pragma once
#include"stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"
#include "sky_map.h"
#include "Graphics/RenderTarget.h"
#include"PostprocessingRenderer.h"
#include<thread>
#include"Audio/Audio.h"


// ゲームシーン
class SceneGame :public Scene
{
public:
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(const float& elapsedTime) override;

	// 描画処理
	void Render() override;

	//3D用の描画
	void Render3DScene();

	//オフスクリーンレンダリング
	void Offscreen_Rendering();

	//デバック
	void DebugGui();
private:
	//エネミーHPゲージ描画
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	void RenderEnemyRockOn(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);
private:
	std::unique_ptr<Stage> stage;
	std::unique_ptr<Player> player;
	std::unique_ptr<CameraController> cameraController;
	std::unique_ptr<Sprite> gauge;
	std::unique_ptr<Sprite> back;
	std::unique_ptr<Sprite> sprRock;

	std::unique_ptr<Sprite>expl;
	std::unique_ptr<Sprite>mask;
	std::unique_ptr<AudioSource>bgm;
	
private:
	//オフスクリーンレンダリング用描画ターゲット
	std::unique_ptr<RenderTarget> renderTarget;
	//ポストプロセス
	std::unique_ptr<PostprocessingRenderer> postprocessingRneder;
	//スカイボックス
	std::shared_ptr<sky_map>skyMap;
	//ライト関係
	DirectX::XMFLOAT4 directional_light = { -0.6f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）
	DirectX::XMFLOAT4 stageColor = { 0.486f,0.054f,0.517f,1.0f };
	DirectX::XMFLOAT4 modelColor = { 0.396f,0.411f,0.278f,1.0f };
	DirectX::XMFLOAT4 lightColor = { 1.0f,1.0f,1.0f,1.0f };
	bool Rock_ON = false;
	float time = 60.0f;
	std::unique_ptr<Sprite>finish;
	float Scene_Change_Timer = 2.0f;
	float a = 0.0f;
};