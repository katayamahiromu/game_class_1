#pragma once
#include "Scene.h"
#include "Graphics/Sprite.h"
#include <memory>
class SceneClear:public Scene
{
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
};