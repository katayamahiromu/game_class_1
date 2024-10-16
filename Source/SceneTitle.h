#pragma once

#include"Graphics//Sprite.h"
#include"Scene.h"
#include"Audio/Audio.h"
#include<memory>

//�^�C�g���V�[��
class SceneTitle :public Scene
{
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
private:
	Sprite* sprite = nullptr;
	std::unique_ptr<Sprite>title;
	std::unique_ptr<AudioSource> Cdur;
	std::unique_ptr<SubMixVoice> test;
	std::unique_ptr<Sprite> Push_Enter;
};