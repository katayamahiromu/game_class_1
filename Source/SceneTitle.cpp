#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneGame.h"
#include"SceneClear.h"
#include"SceneManager.h"
#include"Input/Input.h"
#include"SceneLoading.h"

bool isVisible = true;  // �������\������邩�ǂ����𐧌䂷��t���O
auto lastToggleTime = std::chrono::steady_clock::now();  // �Ō�ɐ؂�ւ����s��ꂽ���Ԃ��L�^
std::chrono::milliseconds blinkInterval(500);  // 500�~���b���Ƃɓ_��

//������
void SceneTitle::Initialize()
{
	//�X�v���C�g������
	//sprite = new Sprite("Data/Sprite/Title.png");
	title = std::make_unique<Sprite>("Data/Sprite/umi.png");
	playSpr = std::make_unique<Sprite>("Data/Sprite/PlayGame.png");
	controlSpr = std::make_unique<Sprite>("Data/Sprite/Control.png");
	renderControl = std::make_unique<Sprite>("Data/Sprite/sousa.png");
	selectMark = std::make_unique<Sprite>("Data/Sprite/selectMark.png");

	test = Audio::Instance().MakeSubMix();
	Cdur = Audio::Instance().LoadAudioSource("Data/Audio/SE.wav");
	Cdur->Set_Submix_voice(test->Get_Submix_Voice());
	test->SetVolum(1.0f);
	test->equalizer();
	Cdur->Play(false);
	Push_Enter = std::make_unique<Sprite>("Data/Font/font0.png");
}

//�I����
void SceneTitle::Finalize()
{
	//�X�v���C�g�I����
	//if (sprite != nullptr)
	//{
	//	delete sprite;
	//	sprite = nullptr;
	//}
	
}

//�X�V����
void SceneTitle::Update(const float& elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//�Ȃɂ��{�^������������Q�[���V�[���ւ̐؂�ւ�

	
	// �㉺�L�[�I��
	if (GetAsyncKeyState('W') & 0x8000)
	{
		selectIndex--;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		selectIndex++;
	}
	// ����
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
		if (gamePad.GetButtonDown() & GamePad::BTN_SPACE) {
			SceneManager::instance().ChengeScene(new SceneLoading(new SceneGame));
		}
		break;
	case 1:
		selectPos = 630;
		if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
		{
			renderSpr =!renderSpr ;
		}
		
		break;
	}
	auto currentTime = std::chrono::steady_clock::now();
	if (currentTime - lastToggleTime >= blinkInterval)
	{
		isVisible = !isVisible;  // �\����Ԃ��g�O���i�\���E��\����؂�ւ���j
		lastToggleTime = currentTime;  // �^�C���X�^���v���X�V
	}
	
}

//�`�揈��
void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	ID3D11BlendState* bs = graphics.GetBlendState();

	//��ʃN���A�������_�[�^�[�Q�b�g
	FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	dc->OMSetBlendState(bs, nullptr, 0xFFFFFFFF);

	ImGui::Begin("posusus");
	ImGui::DragFloat2("pos", &pos.x);
	ImGui::DragFloat("width", &width);
	ImGui::DragFloat("height", &height);

	ImGui::End();
	//2D�X�v���C�g�`��
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(title->GetTextureWidth());
		float textureHeight = static_cast<float>(title->GetTextureHeight());

		float playtexWidth = static_cast<float>(playSpr->GetTextureWidth());
		float playtexHeight = static_cast<float>(playSpr->GetTextureHeight());

		float controltexWidth = static_cast<float>(controlSpr->GetTextureWidth());
		float controltexHeight = static_cast<float>(controlSpr->GetTextureHeight());

		//�^�C�g���X�v���C�g
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
	// ���݂̎��Ԃ��擾
	auto currentTime = std::chrono::steady_clock::now();

	// �Ō�ɐ؂�ւ����s��ꂽ���Ԃ���̌o�ߎ��Ԃ��v�Z
	if (currentTime - lastToggleTime >= blinkInterval)
	{
		isVisible = !isVisible;  //�_�ł����邩�����Ȃ���
		lastToggleTime = currentTime;
	}
}
