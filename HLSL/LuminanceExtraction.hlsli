struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

cbuffer CBLuminanceExraction : register(b0)
{
	float threshold; //���P�x���o�̂��߂�臒l
	float intensity; //�u���[���̋��x
	float2 dummy;
};

float RGB2Luminance(float3 rgb)
{
	static const float3 luminanceValue = float3(0.299f, 0.587f, 0.114f);
	return dot(luminanceValue, rgb);
}