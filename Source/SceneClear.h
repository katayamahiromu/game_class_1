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

	//‰Šú‰»
	virtual void Initialize() override;

	//I—¹‰»
	virtual void Finalize() override;

	//XVˆ—
	virtual void Update(const float& elapsedTime) override;

	//•`‰æˆ—
	virtual void Render() override;
private:
	std::unique_ptr<Sprite>clear;
	std::unique_ptr<AudioSource>bgm;
	std::unique_ptr<Font>font;
};