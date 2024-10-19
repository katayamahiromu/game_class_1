#pragma once

#include"Graphics/Sprite.h"
#include"Scene.h"
#include<thread>

//ロードディングシーン
class SceneLoading :public Scene
{
public:
	SceneLoading(Scene*nextScene):nextScene(nextScene){}
	~SceneLoading() override{}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(const float& elapsedTime) override;

	// 描画処理
	void Render() override;

	//デバッグ
	void Gui();
private:
	//ローディングスレッド
	static void LoadingThread(SceneLoading* scene);
private:
	Sprite* sprite = nullptr;
	std::unique_ptr<Sprite>pet;
	std::unique_ptr<Sprite>pet2;
	std::unique_ptr<Sprite>meter;
	std::unique_ptr<Sprite>meter2;
	float angle = 0.0f;
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;


	DirectX::XMFLOAT2 m_pos;
	DirectX::XMFLOAT2 m_texsize;
	DirectX::XMFLOAT2 m2_pos;
	DirectX::XMFLOAT2 m2_size;
	DirectX::XMFLOAT2 m2_texsize;


	float alp;
};