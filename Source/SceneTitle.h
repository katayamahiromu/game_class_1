#pragma once

#include"Graphics//Sprite.h"
#include"Scene.h"
#include"Audio/Audio.h"
#include<memory>

//タイトルシーン
class SceneTitle :public Scene
{
public:
	SceneTitle(){}
	~SceneTitle(){}

	//初期化
	virtual void Initialize() override;

	//終了化
	virtual void Finalize() override;

	//更新処理
	virtual void Update(float elapsedTime) override;

	//描画処理
	virtual void Render() override;
private:
	Sprite* sprite = nullptr;
	std::unique_ptr<AudioSource> Cdur;
	std::unique_ptr<SubMixVoice> test;
};