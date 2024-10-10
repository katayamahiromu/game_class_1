#pragma once

#include"Graphics/Shader.h"
#include"Graphics/Sprite.h"
#include"Graphics/RenderTarget.h"

//シェーダーリソースビューの情報
struct ShaderResourceViewData
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>src;
	int width = 0;
	int height = 0;
};

//ポストプロセス用のレンダラー
class PostprocessingRenderer
{
	//ブルーム情報
	struct BloomData
	{
		//高輝度抽出用情報
		LuminanceExtractionData luminanceExtractionData;

		//色調補正
		ColorGradingData colorGradingData;
	};

public:
	PostprocessingRenderer();
	~PostprocessingRenderer();

	//描画
	void Render(ID3D11DeviceContext* deviceContext);

	//シーン情報の設定
	void SetSceneData(ShaderResourceViewData sryData);

	//デバック情報の開示
	void DrawDebugGUI();

private:
	//シーン描画データ
	ShaderResourceViewData sceneData;

	//描画用スプライト
	std::unique_ptr<Sprite> renderSprite;

	//ブルームデータ
	BloomData bloomData;

	//高輝度抽出テクスチャ
	std::unique_ptr<RenderTarget> luminanceExtractRenderTarget;
	//色調調整用テクスチャ
	std::unique_ptr<RenderTarget> ColorGradingExtractRenderTarget;
};