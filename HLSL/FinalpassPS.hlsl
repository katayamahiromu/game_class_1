#include"Finalpass.hlsli"

//シーンテクスチャ
Texture2D sceneTexture : register(t0);
SamplerState sceneSampler : register(s0);

//ブルームテクスチャ
Texture2D bloomTexture : register(t1);

float4 main(VS_OUT pin) :SV_TARGET
{
	float4 color = sceneTexture.Sample(sceneSampler,pin.texcoord) * pin.color;

	//ブルームテクスチャを加算する
	color.rgb += bloomTexture.Sample(sceneSampler, pin.texcoord) * pin.color;

	//ブルームテクスチャを加算する
	color.rgb += bloomTexture.Sample(sceneSampler, pin.texcoord).rgb;
	return color;
}