#pragma once

#include"Graphics//Sprite.h"
#include "Graphics/Font.h"
#include"Scene.h"
#include"Audio/Audio.h"
#include<memory>
#include <chrono>
//�^�C�g���V�[��
class SceneTitle :public Scene
{
public:
	DirectX::XMFLOAT2 pos = { 0,0 };
	float width = 0.0f;
	float height = 0.0f;
	

public:
	SceneTitle(){}
	~SceneTitle(){}

	//������
	virtual void Initialize() override;

	//�I����
	virtual void Finalize() override;

	//�X�V����
	virtual void Update(const float& elapsedTime) override;

	//�`�揈��
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