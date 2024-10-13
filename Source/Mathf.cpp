#include"Mathf.h"
#include<stdlib.h>

//���`�ۊ�
float Mathf::Leap(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

//�w��͈͂̃����_���l���v�Z����
float Mathf::RandomRange(float min, float max)
{
    // 0����1�̊Ԃ̃����_���Ȓl�𐶐�
    float random01 = static_cast<float>(rand()) / RAND_MAX;

    // min����max�͈̔͂̃����_���Ȓl���v�Z���ĕԂ�
    return min + (max - min) * random01;
}

DirectX::XMFLOAT3 Mathf::Leap(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 target, float t)
{
    DirectX::XMVECTOR vPos = DirectX::XMLoadFloat3(&pos);
    DirectX::XMVECTOR vTarget = DirectX::XMLoadFloat3(&target);

    DirectX::XMFLOAT3 result; 
    DirectX::XMStoreFloat3(&result, DirectX::XMVectorLerp(vPos, vTarget, t));
    return result;
}
