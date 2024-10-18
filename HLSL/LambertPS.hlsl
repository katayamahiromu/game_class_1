#include "Lambert.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);

float4 main(VS_OUT pin) : SV_TARGET
{
	//�n�C�g�}�b�v���獂�����擾
	float height = Height_map.Sample(diffuseMapSamplerState,pin.texcoord).r;
	//���_�ʒu�̕ψ�
	float3 displacedPosition = pin.world_position + float3(0.0, height * 1.0f, 0.0);
	pin.position += float4(displacedPosition, 1.0);
	//�@���}�b�v����xyz�������擾���āi-1 ~ +1�j�̊ԂɃX�P�[�����O
	float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).xyz * 2 - 1;

	//�J�����s�񂩂王���s��̔������
	float3 viewDir;
	viewDir.x = viewProjection[3][1];
	viewDir.y = viewProjection[3][2];
	viewDir.z = viewProjection[3][3];

	viewDir = normalize(viewDir);

	//�ϊ��p��3X3�s���p�ӂ���
	float3x3 CM = {
		normalize(pin.tangent),
		normalize(pin.binormal),
		normalize(pin.normal),
	};

	//�}�e���A���萔
	//float3 ka = float3(1, 1, 1);
	//float3 kd = float3(1, 1, 1);
	//float3 ks = float3(1, 1, 1);
	float shiness = 512;

	float3 N = normalize(mul(normal, CM));
	float3 L = normalize(lightDirection.xyz);
	float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);

	//�����̌v�Z
	//float3 ambient = ka * ambientLightColor;
	float3 ambient = stageColor;

	//�������ʂ̓K��
	float2 correctedUV = ParallaxMapping(pin.texcoord, viewDir, 0.05, -0.02);
	float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, correctedUV) * pin.color;

	//���s�����̃��C�e�B���O�v�Z
	float3 directionalSpecular = CalcPhongSpecular(N, L, lightColor.rgb, E, shiness, diffuseColor.rgb);


	float4 color = float4(ambient, diffuseColor.a);
	color.rgb += diffuseColor.rgb;
	color.rgb += directionalSpecular;
	return color;
}