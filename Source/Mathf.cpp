#include"Mathf.h"
#include<stdlib.h>

//線形保管
float Mathf::Leap(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

//指定範囲のランダム値を計算する
float Mathf::RandomRange(float min, float max)
{
    // 0から1の間のランダムな値を生成
    float random01 = static_cast<float>(rand()) / RAND_MAX;

    // minからmaxの範囲のランダムな値を計算して返す
    return min + (max - min) * random01;
}
