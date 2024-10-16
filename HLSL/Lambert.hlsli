struct VS_OUT
{
	float4 position : SV_POSITION;
	float3 world_position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
};

cbuffer CbScene : register(b0)
{
	float4 viewPosition;
	row_major float4x4	viewProjection;
	float4				ambientLightColor;

	float4				lightDirection;
	float4				lightColor;
};

#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
	row_major float4x4	boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
	float4				materialColor;
};


float3 CalcPhongSpecular(float3 normal, float3 lightVector, float3 lightColor,
	float3 eveVector, float shininess, float3 ks)
{
	float3 R = reflect(lightVector, normal);
	float d = max(dot(eveVector, R), 0);
	d = pow(d, shininess);
	return lightColor * d * ks;
}

Texture2D Height_map : register(t2);
SamplerState diffuseMapSamplerState : register(s0);

float2 ParallaxMapping(float2 uv, float3 viewDir, float scale, float bias) {
	// ビューベクトルに沿った視差補正
	// ハイトマップから高さを取得
	float height = Height_map.Sample(diffuseMapSamplerState,uv).r;
	float parallax = height * scale + bias;
	return uv + viewDir.xy * parallax;
}
