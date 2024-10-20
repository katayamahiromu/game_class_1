#pragma once
#include "Scene.h"
#include "Graphics/Sprite.h"
#include <memory>
class SceneClear:public Scene
{
public:
	SceneClear(){}
	~SceneClear(){}

	//������
	virtual void Initialize() override;

	//�I����
	virtual void Finalize() override;

	//�X�V����
	virtual void Update(const float& elapsedTime) override;

	//�`�揈��
	virtual void Render() override;
private:
	std::unique_ptr<Sprite>clear;
};