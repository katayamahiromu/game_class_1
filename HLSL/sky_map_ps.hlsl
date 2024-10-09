#include "sky_map.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
SamplerState sampler_states[3] : register(s0);
Texture2D skymap : register(t0); // latitude-longitude mapped texture

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 R = mul(float4((pin.texcoord.x * 2.0) - 1.0, 1.0 - (pin.texcoord.y * 2.0), 1, 1), inverse_view_projection);
	R /= R.w;
	float3 v = normalize(R.xyz);

	// Blinn/Newell Latitude Mapping
	const float PI = 3.14159265358979;
	float2 sample_point;
	sample_point.x = (atan2(v.z, v.x) + PI) / (PI * 2.0);
	sample_point.y = 1.0 - ((asin(v.y) + PI * 0.5) / PI);

	const float lod = 0;
	return skymap.SampleLevel(sampler_states[0], sample_point, lod);
}

