#include"sprite.hlsli"

Texture2D color_map : register(t0);
SamplerState sampler0 : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	float4  color = color_map.Sample(sampler0, pin.texcoord) * pin.color;
	float isBlack = step(0.0001, length(color.rgb));
	color.a = lerp(0.0, 1.0, isBlack);

	return color;
}