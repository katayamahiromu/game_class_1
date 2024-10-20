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
	std::unique_ptr<Sprite>pet;
	std::unique_ptr<Sprite>pet2;
	std::unique_ptr<Sprite>meter;
	std::unique_ptr<Sprite>meter2;
	float angle = 0.0f;
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;


	DirectX::XMFLOAT2 m_pos;
	DirectX::XMFLOAT2 m_texsize;
	DirectX::XMFLOAT2 m2_pos;
	DirectX::XMFLOAT2 m2_size;
	DirectX::XMFLOAT2 m2_texsize;


	float alp;
};