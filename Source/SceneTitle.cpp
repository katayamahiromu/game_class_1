#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneGame.h"
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
	font = std::make_unique<Font>("Data/Font/MS Gothic.fnt",1024);

	test = Audio::Instance().MakeSubMix();
	Cdur = Audio::Instance().LoadAudioSource("Data/Audio/SE.wav");
	Cdur->Set_Submix_voice(test->Get_Submix_Voice());
	test->SetVolum(1.0f);
	test->equalizer();
	Cdur->Play(false);
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

	if (gamePad.GetButtonDown() & GamePad::BTN_SPACE) {
		SceneManager::instance().ChengeScene(new SceneLoading(new SceneGame));
		//SceneManager::instance().ChengeScene(new SceneGame);
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

	//2D�X�v���C�g�`��
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(title->GetTextureWidth());
		float textureHeight = static_cast<float>(title->GetTextureHeight());

		//�^�C�g���X�v���C�g
		title->Render(dc,
			0, 0, screenWidth, screenHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1
		);
		UpdateBlink();
		if (isVisible)
		{
			font->Begin(dc);
			font->Draw(540, 500, L"Press EnterKey");
			font->End(dc);
		}

		/*const char buf[] = "Press EnterKay";

		for (const char c : buf)
		{
			sprite->Render(dc,
				0, 0, 100, 100,
				16 *  (c & 0x0F), 16*(c >> 4), 16, 16,
				0,
				1, 1, 1, 1
			);
		}*/

	//	sprite->Render(dc,
	//		0, 0, screenWidth, screenHeight,
	//		0, 0, textureWidth, textureHeight,
	//		0,
	//		1, 1, 1, 1
	//	);
	//	sprite->textout(dc,
	//		"Press EnterKey",
	//		100, 100,
	//		100, 100,
	//		0.5f, 0.5f, 0.5f, 1.0f);

		Push_Enter->textout(dc, 
			"P", 200, 500, 100, 100,
			1.0f, 1.0f, 0.0f, 1.0f);
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
