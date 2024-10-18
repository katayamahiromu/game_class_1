#pragma once
#include"Graphics/Shader.h"
#include"Character.h"
#include"Collision.h"

//�G�l�~�[
class Enemy :public Character
{
public:
	Enemy(){}
	~Enemy()override{}
	//�X�V����
	virtual void Update(float& elapsedTime) = 0;
	//�`�揈��
	virtual void Render(ID3D11DeviceContext* dc, Shader* sheder) = 0;
	//�f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();
	//�j��
	void Destroy();

	DirectX::XMFLOAT3 Hit_Pos(int i) { return hit_pos[i]; }
	float Hit_radius(int i) { return c_radius[i]; }
protected:
	DirectX::XMFLOAT3 hit_pos[4] = {
		{1.364f,9.156f,10.253f},
		{-3.831f,9.156f,1.076f},
		{5.584f,6.883f,1.076f},
		{-1.883f,7.208f,-8.734f}
	};
	float c_radius[4] = { 7.472f,6.9414f,7.472f,10.037f };
};