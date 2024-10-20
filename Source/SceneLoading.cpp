#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include"SceneLoading.h"
#include"SceneManager.h"
#include"SceneGame.h"
#include<time.h>

//初期化
void SceneLoading::Initialize()
{
	//スプライトの初期化
	sprite = new Sprite("Data/Sprite/LoadingIcon.png");

	loading = std::make_unique<Sprite>("Data/Sprite/load.png");
	loading2 = std::make_unique<Sprite>("Data/Sprite/load2.png");
	loading3 = std::make_unique<Sprite>("Data/Sprite/load3.png");
	loading4 = std::make_unique<Sprite>("Data/Sprite/load4.png");


	//スレッド開始
	thread = new std::thread(SceneLoading::LoadingThread, this);

	flg = false;
	scenechangeflg = false;
	ctimer = 0.0f;
	nowscene = 0;
	scenechangecount = 0;
}

//終了化
void SceneLoading::Finalize()
{
	//スレッド終了化
	thread->join();
	if (thread != nullptr)
	{
		delete thread;
		thread = nullptr;
	}
	//スプライト初期化
	if(sprite!=nullptr)
	{
		delete sprite;
		sprite = nullptr;
	}
}

//更新処理
void SceneLoading::Update(const float& elapsedTime)
{
	constexpr float speed = 0.5;
	ctimer += speed * elapsedTime;
	srand((unsigned int)time(NULL));
	rnd = rand() % 4;
	if (!flg)
	{
		nowscene = rnd;
		flg = true;
	}
	if (ctimer > 1.5f)
	{
		ctimer = 0.0f;
		nowscene += 1;
		scenechangecount += 1;
		if (nowscene > 3)nowscene = 0;
	}
	if (scenechangecount > 5)scenechangeflg = true;
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (nextScene->IsReady() && scenechangeflg)
	{
		SceneManager::instance().ChengeScene(nextScene);
	}

}

//描画処理
void SceneLoading::Render()
{

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//画面クリア＆ターゲット設定
	FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	//2Dスプライト描画
	{
		//画面右下にローディングアイコン描画
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(sprite->GetTextureWidth());
		float textureHeight = static_cast<float>(sprite->GetTextureHeight());
		float positionX = screenWidth - textureWidth;
		float positionY = screenHeight - textureHeight;
		if (nowscene == 0)
		{
			loading->Render(dc,
				0, 0, 1280, 720,
				0, 0, 5568, 4872,
				0,
				1, 1, 1, 1
			);
		}
		if (nowscene == 1)
		{
			loading2->Render(dc,
				0, 0, 1280, 720,
				0, 0, 5568, 4872,
				0,
				1, 1, 1, 1
			);
		}
		if (nowscene == 2)
		{
			loading3->Render(dc,
				0, 0, 1280, 720,
				0, 0, 5568, 4872,
				0,
				1, 1, 1, 1
			);
		}
		if (nowscene == 3)
		{
			loading4->Render(dc,
				0, 0, 1280, 720,
				0, 0, 5568, 4872,
				0,
				1, 1, 1, 1
			);
		}
	}

	
	//Gui();
}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM関連の初期化でスレッド毎に呼ぶ必要がある
	CoInitialize(nullptr);

	scene->nextScene->Initialize();

	//スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	//次のシーンの準備完了設定
	scene->nextScene->SetReady();
}

void SceneLoading::Gui()
{
	ImGui::Begin("Gui");
	ImGui::InputFloat("ctimer", &ctimer);

	ImGui::End();
}