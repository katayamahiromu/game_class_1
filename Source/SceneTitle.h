#pragma once

#include"Graphics//Sprite.h"
#include "Graphics/Font.h"
#include"Scene.h"
#include"Audio/Audio.h"
#include<memory>
#include <chrono>
//タイトルシーン
class SceneTitle :public Scene
{
public:
	DirectX::XMFLOAT2 pos = { 0,0 };
	float width = 0.0f;
	float height = 0.0f;
	

public:
	SceneTitle(){}
	~SceneTitle(){}

	//初期化
	virtual void Initialize() override;

	//終了化
	virtual void Finalize() override;

	//更新処理
	virtual void Update(const float& elapsedTime) override;

	//描画処理
	virtual void Render() override;

	void UpdateBlink();
private:
	//Sprite* sprite = nullptr;
	std::unique_ptr<Sprite>title;
	std::unique_ptr<Sprite>playSpr;
	std::unique_ptr<Sprite>controlSpr;
	std::unique_ptr<AudioSource> Cdur;
	std::unique_ptr<SubMixVoice> test;
};