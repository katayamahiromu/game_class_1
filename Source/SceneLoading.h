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

	std::unique_ptr<Sprite>loading;
	std::unique_ptr<Sprite>loading2;
	std::unique_ptr<Sprite>loading3;
	std::unique_ptr<Sprite>loading4;
	float angle = 0.0f;
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;

	int rnd;
	int nowscene;
	bool flg = false;
	bool scenechangeflg;
	int  scenechangecount;
	float ctimer;
};