struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

cbuffer CBLuminanceExraction : register(b0)
{
	float threshold; //高輝度抽出のための閾値
	float intensity; //ブルームの強度
	float2 dummy;
};

float RGB2Luminance(float3 rgb)
{
	static const float3 luminanceValue = float3(0.299f, 0.587f, 0.114f);
	return dot(luminanceValue, rgb);
}