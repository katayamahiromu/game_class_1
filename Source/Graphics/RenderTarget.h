#pragma once

#include<wrl.h>
#include<d3d11.h>

//描画ターゲットバッファ
class RenderTarget
{
public:
	RenderTarget(UINT width, UINT height, DXGI_FORMAT format);
	~RenderTarget(){}

	//シェーダーリソースビュー取得
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView()
	{
		return shaderResourceView;
	}

	//レンダーターゲットビュー取得
	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRenderTargetView()
	{
		return renderTargetView;
	}

	//テクスチャ幅取得
	inline int Getwidth() { return texture2dDesc.Width; }
	//テクスチャ高さ取得
	inline int GetHeight() { return texture2dDesc.Height; }
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	D3D11_TEXTURE2D_DESC texture2dDesc;
};