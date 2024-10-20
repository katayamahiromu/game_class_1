#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneGame.h"
#include"SceneClear.h"
#include"SceneManager.h"
#include"Input/Input.h"
#include"SceneLoading.h"

bool isVisible = true;  // 文字が表示されるかどうかを制御するフラグ
auto lastToggleTime = std::chrono::steady_clock::now();  // 最後に切り替えが行われた時間を記録
std::chrono::milliseconds blinkInterval(500);  // 500ミリ秒ごとに点滅

//初期化
void SceneTitle::Initialize()
{
	//スプライト初期化
	//sprite = new Sprite("Data/Sprite/Title.png");
	title = std::make_unique<Sprite>("Data/Sprite/umi.png");
	playSpr = std::make_unique<Sprite>("Data/Sprite/PlayGame.png");
	controlSpr = std::make_unique<Sprite>("Data/Sprite/Control.png");
	renderControl = std::make_unique<Sprite>("Data/Sprite/sousa.png");
	selectMark = std::make_unique<Sprite>("Data/Sprite/selectMark.png");

	
	Push_Enter = std::make_unique<Sprite>("Data/Font/font0.png");
	bgm = Audio::Instance().LoadAudioSource("Data/Audio/1 The Clock Falls on a City of Ashes.wav");
	bgm->Play(true);
	select_se = Audio::Instance().LoadAudioSource("Data/Audio/select.wav");
	select_decide = Audio::Instance().LoadAudioSource("Data/Audio/select_decide.wav");
}

//終了化
void SceneTitle::Finalize()
{
	//スプライト終了化
	//if (sprite != nullptr)
	//{
	//	delete sprite;
	//	sprite = nullptr;
	//}
	
}

//更新処理
void SceneTitle::Update(const float& elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//なにかボタンを押したらゲームシーンへの切り替え

	
	// 上下キー選択
	if (GetAsyncKeyState('W') & 0x8000)
	{
		select_se->DC_Play();
		selectIndex--;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		select_se->DC_Play();
		selectIndex++;
	}
	// 制限
	if (selectIndex < 0)
	{
		selectIndex = 0;
	}
	if (selectIndex > 1)
	{
		selectIndex = 1;
	}
	if (selectIndex == 0)renderSpr = false;
	switch (selectIndex)
	{
	case 0:
		selectPos = 470;
		if (gamePad.GetButtonDown() & GamePad::BTN_ENTER) {
			select_decide->DC_Play();
			SceneManager::instance().ChengeScene(new SceneLoading(new SceneGame));
			
		}
		break;
	case 1:
		selectPos = 630;
		if (gamePad.GetButtonDown() & GamePad::BTN_ENTER)
		{
			select_decide->DC_Play();
			renderSpr =!renderSpr ;
		}
		
		break;
	}
	auto currentTime = std::chrono::steady_clock::now();
	if (currentTime - lastToggleTime >= blinkInterval)
	{
		isVisible = !isVisible;  // 表示状態をトグル（表示・非表示を切り替える）
		lastToggleTime = currentTime;  // タイムスタンプを更新
	}
	
}

//描画処理
void SceneTitle::Render()
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

	dc->OMSetBlendState(bs, nullptr, 0xFFFFFFFF);

	//2Dスプライト描画
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(title->GetTextureWidth());
		float textureHeight = static_cast<float>(title->GetTextureHeight());

		float playtexWidth = static_cast<float>(playSpr->GetTextureWidth());
		float playtexHeight = static_cast<float>(playSpr->GetTextureHeight());

		float controltexWidth = static_cast<float>(controlSpr->GetTextureWidth());
		float controltexHeight = static_cast<float>(controlSpr->GetTextureHeight());

		//タイトルスプライト
		title->Render(dc,
			0, 0, screenWidth, screenHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1
		);
		playSpr->Render(dc,
			455, 450, 370, 80,
			0, 0, playtexWidth, playtexHeight,
			0,
			1, 1, 1, 1);

		controlSpr->Render(dc,
			470, 600, 370, 80,
			0, 0, controltexWidth, controltexHeight,
			0,
			1, 1, 1, 1);

		selectMark->Render(dc,
			425,selectPos, 50,30,
			0, 0, 269, 187,
			0,
			1, 1, 1, 1);
		if (renderSpr)
		{
			renderControl->Render(dc,
				145,70, 1000, 600,
				0, 0, 500,500,
				0,
				1, 1, 1, 1);
		}
		
		/*font->Begin(dc);
		font->Draw(600, 500, L"Play Game",1.0f);
		font->End(dc);

		font->Begin(dc);
		font->Draw(610, 550, L"Tutorial", 1.0f);
		font->End(dc);*/
		

	

	
	}

}

void SceneTitle::UpdateBlink()
{
	// 現在の時間を取得
	auto currentTime = std::chrono::steady_clock::now();

	// 最後に切り替えが行われた時間からの経過時間を計算
	if (currentTime - lastToggleTime >= blinkInterval)
	{
		isVisible = !isVisible;  //点滅させるかさせないか
		lastToggleTime = currentTime;
	}
}
