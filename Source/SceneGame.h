#pragma once
#include"stage.h"
#include"Player.h"
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"
#include "sky_map.h"
#include "Graphics/RenderTarget.h"
#include"PostprocessingRenderer.h"
#include<thread>
#include"Audio/Audio.h"


// �Q�[���V�[��
class SceneGame :public Scene
{
public:
	~SceneGame() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(const float& elapsedTime) override;

	// �`�揈��
	void Render() override;

	//3D�p�̕`��
	void Render3DScene();

	//�I�t�X�N���[�������_�����O
	void Offscreen_Rendering();

	//�f�o�b�N
	void DebugGui();
private:
	//�G�l�~�[HP�Q�[�W�`��
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	void RenderEnemyRockOn(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);
private:
	std::unique_ptr<Stage> stage;
	std::unique_ptr<Player> player;
	std::unique_ptr<CameraController> cameraController;
	std::unique_ptr<Sprite> gauge;
	std::unique_ptr<Sprite> back;
	std::unique_ptr<Sprite> sprRock;

	std::unique_ptr<Sprite>expl;
	std::unique_ptr<Sprite>mask;
	std::unique_ptr<AudioSource>bgm;
	
private:
	//�I�t�X�N���[�������_�����O�p�`��^�[�Q�b�g
	std::unique_ptr<RenderTarget> renderTarget;
	//�|�X�g�v���Z�X
	std::unique_ptr<PostprocessingRenderer> postprocessingRneder;
	//�X�J�C�{�b�N�X
	std::shared_ptr<sky_map>skyMap;
	//���C�g�֌W
	DirectX::XMFLOAT4 directional_light = { -0.6f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j
	DirectX::XMFLOAT4 stageColor = { 0.486f,0.054f,0.517f,1.0f };
	DirectX::XMFLOAT4 modelColor = { 0.396f,0.411f,0.278f,1.0f };
	DirectX::XMFLOAT4 lightColor = { 1.0f,1.0f,1.0f,1.0f };
	bool Rock_ON = false;
	float time = 60.0f;
	std::unique_ptr<Sprite>finish;
	float Scene_Change_Timer = 2.0f;
	float a = 0.0f;
};