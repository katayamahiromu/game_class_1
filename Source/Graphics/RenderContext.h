#pragma once
#include<d3d11.h>
#include <DirectXMath.h>

// 高輝度抽出用情報
struct LuminanceExtractionData
{
	float threshold = 0.5f; //閾値
	float intensity = 1.0f; //ブルームの強度
	DirectX::XMFLOAT2 dummy2 = {0.0f,0.0f};
};

//色調補正情報
struct ColorGradingData
{
	float hueShift = 0;// 色相調整
	float saturation = 1;// 彩度調整
	float brightness = 0;// 明度調整
	float dummy = 0;
};

//ポストエフェクト用の最終パス用情報
struct FinalpassData
{
	//ブルームテクスチャ
	ID3D11ShaderResourceView* bloomTexture;
};


// レンダーコンテキスト
struct RenderContext
{
	//カメラ情報
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;

	//ライト情報
	DirectX::XMFLOAT4		ambientLightColor;
	DirectX::XMFLOAT4		lightDirection;
	DirectX::XMFLOAT4		lightColor;

	//シェーダーゲープロベースから移植
	ID3D11DeviceContext* deviceContext;
	//高輝度抽出用情報
	LuminanceExtractionData luminanceExtractionData;
	//色調補正情報
	ColorGradingData colorGrandingData;
	//最終パス用情報
	FinalpassData finalpassData;
};