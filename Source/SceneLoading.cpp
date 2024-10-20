#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include"SceneLoading.h"
#include"SceneManager.h"
#include"SceneGame.h"

//初期化
void SceneLoading::Initialize()
{
	//スプライトの初期化
	sprite = new Sprite("Data/Sprite/LoadingIcon.png");
	pet = std::make_unique<Sprite>("Data/Sprite/pet.png");

	pet2 = std::make_unique<Sprite>("Data/Sprite/emptybottle.png");

	meter = std::make_unique<Sprite>("Data/Sprite/meter.png");

	meter2 = std::make_unique<Sprite>("Data/Sprite/meter2.png");
	//スレッド開始
	thread = new std::thread(SceneLoading::LoadingThread, this);
	alp = 0;
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
	GamePad& gamePad = Input::Instance().GetGamePad();
	constexpr float speed = 0.5;
	constexpr float texspeed = 100;
	constexpr float tex2speed = 500;
	m_texsize.y -= texspeed * elapsedTime;
	if (m_texsize.y <= -320)m_texsize.y = -320;
	if (m_texsize.y <= -319)
	{
		m2_texsize.y -= tex2speed * elapsedTime;
		alp -= speed * elapsedTime;
	}
	if (m2_texsize.y <= -345)m2_texsize.y = -345;
	//次のシーンが完了したらシーンを切り替える
	//if (nextScene->IsReady())
	//{
	//	m_texsize.y = -320;
	//}
	if (nextScene->IsReady() &&(gamePad.GetButtonDown() & GamePad::BTN_SPACE))
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

	/*	sprite->Render(dc,
			positionX, positionY, textureWidth, textureHeight,
			0, 0, textureWidth, textureHeight,
			angle,
			1, 1, 1, 1
		);*/

		if (m_texsize.y > -319)
		{
			meter->Render(dc,
				-78, 720, 340, m_texsize.y,
				0, 0, 340, m_texsize.y,
				0,
				1, 1, 1, 1
			);
			pet2->Render(dc,
				-78, 380, 340, 340,
				0,0, 340, 340,
				0,
				1, 1, 1, 1
			);
		}
		else
		{
			pet->Render(dc,
				-78, 380, 340, 340,
				0, 0, 340, 340,
				0,
				1, 1, 1, 1
			);
			meter2->Render(dc,
				34, 430, 71, m2_texsize.y,
				0, 0, 189, m2_texsize.y,
				0,
				1, 1, 1, 1
			);
		}
		//タイトルスプライト
		/*pet->Render(dc,
			0, 0, screenWidth, screenHeight,
			0, 0, 1920, 1080,
			0,
			1, 1, 1, alp
		);*/
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

	ImGui::DragFloat2("m_pos", &m_pos.x);

	ImGui::DragFloat2("m2_pos", &m2_pos.x);
	ImGui::DragFloat2("m2_size", &m2_size.x);
	ImGui::DragFloat2("m2_texsize", &m2_texsize.x);

	ImGui::End();
}