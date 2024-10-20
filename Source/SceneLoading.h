#pragma once

#include"Graphics/Sprite.h"
#include"Scene.h"
#include<thread>

//���[�h�f�B���O�V�[��
class SceneLoading :public Scene
{
public:
	SceneLoading(Scene*nextScene):nextScene(nextScene){}
	~SceneLoading() override{}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(const float& elapsedTime) override;

	// �`�揈��
	void Render() override;

	//�f�o�b�O
	void Gui();
private:
	//���[�f�B���O�X���b�h
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