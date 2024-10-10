#pragma once
#include"stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"

#include "Graphics/RenderTarget.h"
#include"PostprocessingRenderer.h"


// ゲームシーン
class SceneGame:public Scene
{
public:
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

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
private:
	Stage*  stage  = nullptr;
	Player* player = nullptr;
	CameraController* cameraController = nullptr;
	Sprite* gauge = nullptr;
	Sprite* back = nullptr;
private:
	//オフスクリーンレンダリング用描画ターゲット
	std::unique_ptr<RenderTarget> renderTarget;
	//ポストプロセス
	std::unique_ptr<PostprocessingRenderer> postprocessingRneder;

	//ライト関係
	DirectX::XMFLOAT4 directional_light = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）
	DirectX::XMFLOAT4 ambientColor = { 0.2f,0.2f,0.2f,1.0f };
	DirectX::XMFLOAT4 lightColor = { 1.0f,1.0f,1.0f,1.0f };
};
