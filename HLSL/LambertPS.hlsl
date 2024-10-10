
#include "Lambert.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
SamplerState diffuseMapSamplerState : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

	//法線マップからxyz成分を取得して（-1 ~ +1）の間にスケーリング
	float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).xyz * 2 - 1;

	//変換用の3X3行列を用意する
	float3x3 CM = {
		normalize(pin.tangent),
		normalize(pin.binormal),
		normalize(pin.normal),
	};

	//マテリアル定数
	float3 ka = float3(1, 1, 1);
	float3 kd = float3(1, 1, 1);
	float3 ks = float3(1, 1, 1);
	float shiness = 128;

	float3 N = normalize(mul(normal, CM));
	float3 L = normalize(lightDirection.xyz);
	float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);

	//環境光の計算
	float3 ambient = ka * ambientLightColor;

	//平行光源のライティング計算
	float3 directionalSpecular = CalcPhongSpecular(N, L, lightColor.rgb, E, shiness, ks);

	float4 color = float4(ambient, diffuseColor.a);
	color.rgb += diffuseColor.rgb;
	color.rgb += directionalSpecular;
	return color;
}

