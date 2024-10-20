#include "SceneClear.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include <Graphics/Graphics.h>
#include "Input/Input.h"
void SceneClear::Initialize()
{
	clear = std::make_unique<Sprite>("Data/Sprite/Gama_Clear.png");
	font = std::make_unique<Font>("Data/Font/MS Gothic.fnt",1024);
	bgm = Audio::Instance().LoadAudioSource("Data/Audio/10 Steam mechanism.wav");
	bgm->Play(true);
}

void SceneClear::Finalize()
{

}

void SceneClear::Update(const float& elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_SPACE) {
		SceneManager::instance().ChengeScene(new SceneTitle);
	}
}

void SceneClear::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	ID3D11BlendState* bs = graphics.GetBlendState();

	//画面クリア＆レンダーターゲット
	FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());
	float textureWidth = static_cast<float>(clear->GetTextureWidth());
	float textureHeight = static_cast<float>(clear->GetTextureHeight());

	clear->Render(dc,
		0, 0, screenWidth, screenHeight,
		0, 0, textureWidth, textureHeight,
		0,
		1, 1, 1, 1);
	
	font->Begin(dc);
	font->Draw(550, 500, L"Push Space", 1.5f);
	font->End(dc);
}
