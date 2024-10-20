#pragma once
#include "Scene.h"
#include "Graphics/Sprite.h"
#include <memory>
#include "Graphics/Font.h"
#include"Audio/Audio.h"
class SceneClear:public Scene
{
	DirectX::XMFLOAT2 pos{ 0,0 };
public:
	SceneClear(){}
	~SceneClear(){}

	//初期化
	virtual void Initialize() override;

	//終了化
	virtual void Finalize() override;

	//更新処理
	virtual void Update(const float& elapsedTime) override;

	//描画処理
	virtual void Render() override;
private:
	std::unique_ptr<Sprite>clear;
	std::unique_ptr<AudioSource>bgm;
	std::unique_ptr<Font>font;
};