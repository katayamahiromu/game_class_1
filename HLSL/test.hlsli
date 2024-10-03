
struct VS_OUT
{
	float4 position : SV_POSITION;
	float3 world_position : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

cbuffer CbScene :register(b0)
{
	float4 viewPosition;
	row_major float4x4 viewProjection;
}

#define MAX_BONES 128

cbuffer CbMesh : register(b1)
{
	row_major boneTransforms[MAX_BONES];
}