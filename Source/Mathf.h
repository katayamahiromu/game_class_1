#pragma once
#include<DirectXMath.h>


//���������Z�p
class Mathf
{
public:
	//���`�⊮
	static float Leap(float a, float b, float t);
	//�J�����p�ɐV���Ɏ����������`�⊮
	static DirectX::XMFLOAT3 Leap(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 target,float t);
	//�w��͈͂̃����_���l���v�Z����
	static float RandomRange(float min, float max);
};
