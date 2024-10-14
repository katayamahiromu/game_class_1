#include"CameraController.h"
#include"Camera.h"
#include"Input/Input.h"
#include<cmath>
#include"Mathf.h"

//�X�V����
void CameraController::Update(float elapsedTime) 
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	//�J�����̉�]���x
	float speed = rollspeed * elapsedTime;

	//�X�e�B�b�N�̓��͒l�ɂ��킹��X����Y������]
	angle.x += ay * speed;
	angle.y += ax * speed;

	//X���̃J������]�𐧌�
	if (angle.x > maxAngleX) 
	{
		angle.x = maxAngleX;
	}
	if (angle.x < mixAngleX)
	{
		angle.x = mixAngleX;
	}

	//Y���̉�]�l��-3.14 ~ 3.14�Ɏ��܂�悤�ɂ���
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}
	
	//�J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	/*DirectX::XMStoreFloat3(&eye, 
		DirectX::XMVectorScale(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&target), DirectX::XMLoadFloat3(&front)), range));*/
	

	eye = Mathf::Leap(Camera::Instance().GetEye(), eye, elapsedTime*1.0f);

	//�J�����̎��_�ƒ����_��ݒ�
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}