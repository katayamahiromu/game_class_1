#pragma once
#include<DirectXMath.h>


//浮動小数算術
class Mathf
{
public:
	//線形補完
	static float Leap(float a, float b, float t);
	//カメラ用に新たに実装した線形補完
	static DirectX::XMFLOAT3 Leap(DirectX::XMFLOAT3 pos,DirectX::XMFLOAT3 target,float t);
	//指定範囲のランダム値を計算する
	static float RandomRange(float min, float max);
};
