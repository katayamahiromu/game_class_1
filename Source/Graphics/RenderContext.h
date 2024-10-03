#pragma once
#include<d3d11.h>
#include <DirectXMath.h>

struct Test
{
	ID3D11ShaderResourceView* base_color;
	ID3D11ShaderResourceView* height;
	ID3D11ShaderResourceView* metallic;
	ID3D11ShaderResourceView* MixedAo;
	ID3D11ShaderResourceView* normal;
	ID3D11ShaderResourceView* roughness;
};

// レンダーコンテキスト
struct RenderContext
{
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		lightDirection;

	//test用のやつ
	Test test;
};