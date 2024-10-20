#pragma once
#include<d3d11.h>
#include <DirectXMath.h>

// ���P�x���o�p���
struct LuminanceExtractionData
{
	float threshold = 0.4f; //臒l
	float intensity = 1.0f; //�u���[���̋��x
	DirectX::XMFLOAT2 dummy2 = {0.0f,0.0f};
};

//�F���␳���
struct ColorGradingData
{
	float hueShift = 113.5f;// �F������
	float saturation = 0.8f;// �ʓx����
	float brightness = 0.66f;// ���x����
	float dummy = 0;
};

//�|�X�g�G�t�F�N�g�p�̍ŏI�p�X�p���
struct FinalpassData
{
	//�u���[���e�N�X�`��
	ID3D11ShaderResourceView* bloomTexture;
};


// �����_�[�R���e�L�X�g
struct RenderContext
{
	//�J�������
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;

	//���C�g���
	DirectX::XMFLOAT4		stageColor;
	DirectX::XMFLOAT4		modelColor;
	DirectX::XMFLOAT4		lightDirection;
	DirectX::XMFLOAT4		lightColor;

	//�}�X�N�p���e�N�X�`��
	ID3D11ShaderResourceView* maskTexture;
	//�m�[�}���}�b�v
	ID3D11ShaderResourceView* normal_map;

	//�V�F�[�_�[�Q�[�v���x�[�X����ڐA
	ID3D11DeviceContext* deviceContext;
	//���P�x���o�p���
	LuminanceExtractionData luminanceExtractionData;
	//�F���␳���
	ColorGradingData colorGrandingData;
	//�ŏI�p�X�p���
	FinalpassData finalpassData;
};