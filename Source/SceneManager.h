#pragma once
#include"Scene.h"

//シーンマネージャー
class SceneManager
{
private:
	SceneManager(){}
	~SceneManager(){}
public:
	//唯一のインスタンス取得
	static SceneManager& instance()
	{
		static SceneManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);
	//描画処理
	void Render();
	//シーンクリア
	void Clear();
	//シーン切り替え
	void ChengeScene(Scene* scene);
private:
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
};
