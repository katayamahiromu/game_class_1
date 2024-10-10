
#include "Lambert.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
SamplerState diffuseMapSamplerState : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

	//�@���}�b�v����xyz�������擾���āi-1 ~ +1�j�̊ԂɃX�P�[�����O
	float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).xyz * 2 - 1;

	//�ϊ��p��3X3�s���p�ӂ���
	float3x3 CM = {
		normalize(pin.tangent),
		normalize(pin.binormal),
		normalize(pin.normal),
	};

	//�}�e���A���萔
	float3 ka = float3(1, 1, 1);
	float3 kd = float3(1, 1, 1);
	float3 ks = float3(1, 1, 1);
	float shiness = 128;

	float3 N = normalize(mul(normal, CM));
	float3 L = normalize(lightDirection.xyz);
	float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);

	//�����̌v�Z
	float3 ambient = ka * ambientLightColor;

	//���s�����̃��C�e�B���O�v�Z
	float3 directionalSpecular = CalcPhongSpecular(N, L, lightColor.rgb, E, shiness, ks);

	float4 color = float4(ambient, diffuseColor.a);
	color.rgb += diffuseColor.rgb;
	color.rgb += directionalSpecular;
	return color;
}

