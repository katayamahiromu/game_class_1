#pragma once
#include<d3d11.h>
#include <DirectXMath.h>

// ���P�x���o�p���
struct LuminanceExtractionData
{
	float threshold = 0.5f; //臒l
	float intensity = 1.0f; //�u���[���̋��x
	DirectX::XMFLOAT2 dummy2 = {0.0f,0.0f};
};

//�F���␳���
struct ColorGradingData
{
	float hueShift = 0;// �F������
	float saturation = 1;// �ʓx����
	float brightness = 0;// ���x����
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
	DirectX::XMFLOAT4		ambientLightColor;
	DirectX::XMFLOAT4		lightDirection;
	DirectX::XMFLOAT4		lightColor;

	//�V�F�[�_�[�Q�[�v���x�[�X����ڐA
	ID3D11DeviceContext* deviceContext;
	//���P�x���o�p���
	LuminanceExtractionData luminanceExtractionData;
	//�F���␳���
	ColorGradingData colorGrandingData;
	//�ŏI�p�X�p���
	FinalpassData finalpassData;
};