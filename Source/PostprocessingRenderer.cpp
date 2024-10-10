#include"PostprocessingRenderer.h"
#include"Graphics/Graphics.h"

PostprocessingRenderer::PostprocessingRenderer()
{
	Graphics& graphics = Graphics::Instance();

	renderSprite = std::make_unique<Sprite>();

	{
		UINT width = static_cast<UINT>(graphics.GetScreenWidth()) / 2;
		UINT height = static_cast<UINT>(graphics.GetScreenHeight()) / 2;

		//高輝度抽出用ターゲットを生成
		luminanceExtractRenderTarget = std::make_unique<RenderTarget>(width, height,DXGI_FORMAT_R8G8B8A8_UNORM);
		//色調調整用ターゲットを生成
		ColorGradingExtractRenderTarget = std::make_unique<RenderTarget>(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
	}
}

PostprocessingRenderer::~PostprocessingRenderer()
{
}

void PostprocessingRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	Graphics& graphics = Graphics::Instance();

	// 現在設定されているバッファを退避して初期化しておく
	UINT			cachedViewportCount{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };
	D3D11_VIEWPORT	cachedViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	cachedRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	cachedDepthStencilView;
	{
		deviceContext->RSGetViewports(&cachedViewportCount, cachedViewports);
		deviceContext->OMGetRenderTargets(1
			, cachedRenderTargetView.ReleaseAndGetAddressOf()
			, cachedDepthStencilView.ReleaseAndGetAddressOf());
	}

	RenderContext rc;
	rc.deviceContext = deviceContext;
	rc.luminanceExtractionData = bloomData.luminanceExtractionData;
	rc.colorGrandingData = bloomData.colorGradingData;



	//抽出した描画対象に色調補正
	{
		ID3D11RenderTargetView *rtv = ColorGradingExtractRenderTarget->GetRenderTargetView().Get();
		FLOAT color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		rc.deviceContext->ClearRenderTargetView(rtv, color);
		rc.deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
		D3D11_VIEWPORT	viewport{};
		viewport.Width = static_cast<float>(ColorGradingExtractRenderTarget->Getwidth());
		viewport.Height = static_cast<float>(ColorGradingExtractRenderTarget->GetHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		rc.deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);

		SpriteShader* shader = graphics.GetShader(SpriteShaderId::ColorGrading);
		shader->Begin(rc);

		//	描画対象を変更
		renderSprite->SetShaderResourceView(sceneData.src, sceneData.width, sceneData.height);
		renderSprite->Update(0, 0, viewport.Width, viewport.Height,
			0, 0, static_cast<float>(sceneData.width), static_cast<float>(sceneData.height),
			0,
			1, 1, 1, 1);
		shader->Draw(rc, renderSprite.get());
		shader->End(rc);
	}

	//	高輝度抽出用バッファに描画先を変更して高輝度抽出
	{
		//	描画先を変更
		ID3D11RenderTargetView* rtv = luminanceExtractRenderTarget->GetRenderTargetView().Get();
		FLOAT color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		rc.deviceContext->ClearRenderTargetView(rtv, color);
		rc.deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
		D3D11_VIEWPORT	viewport{};
		viewport.Width = static_cast<float>(luminanceExtractRenderTarget->Getwidth());
		viewport.Height = static_cast<float>(luminanceExtractRenderTarget->GetHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		rc.deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);

		//	高輝度抽出処理
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::LuminanceExtraction);
		shader->Begin(rc);

		//	描画対象を変更
		renderSprite->SetShaderResourceView(ColorGradingExtractRenderTarget->GetShaderResourceView(),
			ColorGradingExtractRenderTarget->Getwidth(), 
			ColorGradingExtractRenderTarget->GetHeight());
		renderSprite->Update(0, 0, viewport.Width, viewport.Height,
			0, 0, static_cast<float>(ColorGradingExtractRenderTarget->Getwidth()), static_cast<float>(ColorGradingExtractRenderTarget->GetHeight()),
			0,
			1, 1, 1, 1);
		shader->Draw(rc, renderSprite.get());
		shader->End(rc);
	}
	//	元のバッファに戻す
	{
		deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);
		deviceContext->OMSetRenderTargets(1, cachedRenderTargetView.GetAddressOf(), cachedDepthStencilView.Get());
	}
#if true
	//最終パス
	{
		SpriteShader* shader = graphics.GetShader(SpriteShaderId::Finalpass);
		shader->Begin(rc);

		//	描画対象を変更
		renderSprite->SetShaderResourceView(sceneData.src, sceneData.width, sceneData.height);

		renderSprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
			0, 0, static_cast<float>(sceneData.width), static_cast<float>(sceneData.height),
			0,
			1, 1, 1, 1);

		rc.finalpassData.bloomTexture = luminanceExtractRenderTarget->GetShaderResourceView().Get();
		shader->Draw(rc, renderSprite.get());

		shader->End(rc);
	}
#endif
}

//シーン情報の設定
void PostprocessingRenderer::SetSceneData(ShaderResourceViewData srvData)
{
	sceneData = srvData;
}

//デバック情報の開示
void PostprocessingRenderer::DrawDebugGUI()
{
	if (ImGui::TreeNode("PostProcess"))
	{
		if (ImGui::TreeNode("Bloom"))
		{
			ImGui::SliderFloat("threshold", &bloomData.luminanceExtractionData.threshold, 0.0f, 1.0f);
			ImGui::SliderFloat("intensity", &bloomData.luminanceExtractionData.intensity, 0.0f, 10.0f);

			ImGui::SliderFloat("hueShift", &bloomData.colorGradingData.hueShift, 0.0f, +360.0f);
			ImGui::SliderFloat("saturation", &bloomData.colorGradingData.saturation, 0.0f, +2.0f);
			ImGui::SliderFloat("brightness", &bloomData.colorGradingData.brightness, 0.0f, +2.0f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}
