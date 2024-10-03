#pragma once
#include<DirectXMath.h>

//カメラコントロラー
class CameraController
{
public:
	CameraController(){}
	~CameraController(){}

	//更新処理
	void Update(float elapsedTime);

	//ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

	//TPS用のカメラ
	void TPS_Update(float elapsedTiem);
private:
	DirectX::XMFLOAT3 target = { 0,0,0 };
	DirectX::XMFLOAT3 angle  = { 0,0,0 };
	float rollspeed = DirectX::XMConvertToRadians(90);
	float range     = 10.0f;

	float maxAngleX = DirectX::XMConvertToRadians(45);
	float mixAngleX = DirectX::XMConvertToRadians(-45);

};