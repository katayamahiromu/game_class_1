#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include"Camera.h"
#include"EnemyManeger.h"
#include"EnemySlime.h"
#include"EffectManager.h"
#include"Input/Input.h"
#include"StageManager.h"
#include"StageMain.h"
#include"StageMoveFloor.h"
#include"Mathf.h"
#include"Sperm_Manager.h"
#include"UI_Manager.h"


// ������
void SceneGame::Initialize()
{
#if true
	//�X�e�[�W������
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	player = std::make_unique<Player>();
	player->SetPositon(DirectX::XMFLOAT3(0, 100, 0));
	//�J�����R���g���[���[������
	cameraController = std::make_unique<CameraController>();

	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);
	camera.SetEye(DirectX::XMFLOAT3(0.0f, 110.0f, -10.0f));
	//�G�l�~�[������
#if 1
	/*for (int i = 0;i < 2;++i) {
		EnemySlime* slime = new EnemySlime;
		slime->SetPositon(DirectX::XMFLOAT3(i * 2.0f, 100, 5));
		slime->SetTerritory(slime->GetPosition(), 10.0f);
		EnemeyManager::Instance().Register(slime);
	}*/

	for (int i = 0;i < 10;i++)
	{
		Sperm_child* sperm = new Sperm_child;
		sperm->SetPositon(DirectX::XMFLOAT3(i * 2.0f, 100 ,5));
		sperm->SetTerritory(sperm->GetPosition(), 10.0f);
		Sperm_Manager::Instance().Register(sperm);
	}
#else
	for(int i= 0;i<50;++i)
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPositon(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		EnemeyManager::Instance().Register(slime);
	}
#endif
	//UI����
	Sperm_Count_UI* count_ui = new Sperm_Count_UI();
	UI_Manager::Instance().Register(count_ui);
	//�Q�[�W�X�v���C�g
	gauge = std::make_unique<Sprite>();
	//back = new Sprite("Data/Sprite/back.png");
	skyMap = std::make_shared<sky_map>(graphics.GetDevice(), L"Data/SkyMaps/skymap7.png");
	sprRock = std::make_unique<Sprite>("Data/Sprite/Target.png");
#endif

	//�V�����`��^�[�Q�b�g�̐���
	{
		renderTarget = std::make_unique<RenderTarget>(
			static_cast<UINT>(graphics.GetScreenWidth()),
			static_cast<UINT>(graphics.GetScreenHeight()),
			DXGI_FORMAT_R8G8B8A8_UNORM);
	}

	//�|�X�g�v���Z�X�`��N���X����
	postprocessingRneder = std::make_unique<PostprocessingRenderer>();
	//�V�[���e�N�X�`����ݒ肵�Ă���
	ShaderResourceViewData srvData;
	srvData.src = renderTarget->GetShaderResourceView();
	srvData.width = renderTarget->Getwidth();
	srvData.height = renderTarget->GetHeight();
	postprocessingRneder->SetSceneData(srvData);
}

// �I����
void SceneGame::Finalize()
{
#if true
	//�X�e�[�W�I����
	StageManager::Instance().Clear();
	EnemeyManager::Instance().Clear();
	Sperm_Manager::Instance().Clear();
	UI_Manager::Instance().Clear();
#endif
}

// �X�V����
void SceneGame::Update(const float& elapsedTime)
{
#if true
	StageManager::Instance().Update(elapsedTime);
	player->Update(elapsedTime);
	//��ɃJ�������O�G�l�~�[�̏�����D��
	EnemeyManager::Instance().Update(elapsedTime);
	//�J�����R���g���[���[�X�V����
	{
		DirectX::XMFLOAT3 target;
		if (player->Get_Target_Enemy() != nullptr)
		{
			Rock_ON = true;
			target = player->Get_Target_Enemy()->GetPosition();
			target.y += 0.5f;
			cameraController->SetRange(Mathf::Leap(cameraController->GetRange(), 20.0f, elapsedTime));
		}
		else
		{
			Rock_ON = false;
			target = player->GetPosition();
			target.y += 0.5f;
			cameraController->SetRange(Mathf::Leap(cameraController->GetRange(), 10.0f, elapsedTime));
		}
		cameraController->SetTarget(target);
		cameraController->Update(elapsedTime);
	}
	//�G�t�F�N�g�X�V����
	EffectManager::Instace().Update(elapsedTime);
	Sperm_Manager::Instance().Update(elapsedTime);
	UI_Manager::Instance().Update(elapsedTime);
#endif
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	//�����_�[�^�[�Q�b�g�𕡐��g���ݒ�
	ID3D11RenderTargetView* null_render_target_views[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]{};
	dc->OMSetRenderTargets(_countof(null_render_target_views), null_render_target_views, 0);
	ID3D11ShaderResourceView* null_shader_resource_views[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{};
	dc->VSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);
	dc->PSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);

	//3D���f���̕`��
	Render3DScene();
	
	//�I�t�X�N���[�������_�����O
	Offscreen_Rendering();

	// 2D�X�v���C�g�`��
	{
		//RenderEnemyGauge(dc, rc.view, rc.projection);
		RenderEnemyRockOn(dc, Camera::Instance().GetView(), Camera::Instance().GetProjection());
		UI_Manager::Instance().Render();
	}

	// 2D�f�o�b�OGUI�`��
	{
		player->DrawDebugGui();
	}
}


//�G�l�~�[HP�Q�[�W�`��
void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext*dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection
)
{
	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//�S�Ă̓G�̓����HP�Q�[�W��\��
	EnemeyManager& enemyManager = EnemeyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0;i < enemyCount;++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMVECTOR ScreenPosition;
		DirectX::XMFLOAT3 Position;
		Position =  enemy->GetPosition();
		Position.y += enemy->GetHeight();
		ScreenPosition = DirectX::XMVector3Project(
			DirectX::XMLoadFloat3(&Position),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			0.0f,
			1.0f,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, ScreenPosition);
		gauge->Render(dc,
			pos.x - 50.0f * 0.5f, pos.y - 10.0f,
			10.0f * enemy->GetHealth(), enemy->GetMaxhealth(),
			0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f,
			1.0f, 0.0f, 0.0f, 0.0f);
	}

	//�G�l�~�[�z�u����
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		//�}�E�X�J�[�\�����W���擾
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());
		screenPosition.z = 0.0f;
		DirectX::XMFLOAT3  worldPosition = screenPosition;
		DirectX::XMFLOAT3 WorldEnd = { screenPosition.x, screenPosition.y, 1.0f };

		DirectX::XMVECTOR WorldPosition;
		WorldPosition = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&screenPosition),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);

		DirectX::XMVECTOR End;
		End = DirectX::XMVector3Unproject(
			DirectX::XMLoadFloat3(&WorldEnd),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);
		//���C���쐬���ăX�e�[�W�ɑ΂��ă��C�L���X�g����
		DirectX::XMStoreFloat3(&worldPosition, WorldPosition);
		DirectX::XMStoreFloat3(&WorldEnd, End);
		HitResult hit;

		if (StageManager::Instance().RayCast(worldPosition, WorldEnd, hit))
		{
			EnemySlime* slime = new EnemySlime;
			slime->SetPositon(hit.position);
			EnemeyManager::Instance().Register(slime);
		}
		
	}
}


void SceneGame::Render3DScene()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	//�I�t�X�N���[���p�̃����_�[�^�[�Q�b�g��3D�V�[����`��
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();

	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	ID3D11RasterizerState* rs = graphics.GetRasterizerState();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	dc->RSSetState(rs);

	// �`�揈��
	RenderContext rc;

	//�J�����̃p�����[�^�[�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1.0f;

	//�����̏��
	rc.ambientLightColor = ambientColor;
	//�����̊p�x
	rc.lightDirection = directional_light;
	rc.lightColor = lightColor;

	DirectX::XMFLOAT4X4 viewProjection;
	DirectX::XMStoreFloat4x4(&viewProjection, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&camera.GetView()), DirectX::XMLoadFloat4x4(&camera.GetProjection())));
	//skyMap->blit(dc, viewProjection);
	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//�X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);
		player->Render(dc, shader);
		EnemeyManager::Instance().Render(dc, shader);
		Sperm_Manager::Instance().Render(dc, shader);
		shader->End(dc);
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instace().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
		//player->DrawDebugPrimitive();
		//�G�l�~�[�f�o�b�O�v���~�e�B�u
		EnemeyManager::Instance().DrawDebugPrimitive();
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}
	DebugGui();
}

void SceneGame::Offscreen_Rendering()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	// �������ݐ���o�b�N�o�b�t�@�ɕς��ăI�t�X�N���[�������_�����O�̌��ʂ�`�悷��
	{
		ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
		//��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
		FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, &rtv, dsv);


		//�r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT vp = {};
		vp.Width = graphics.GetScreenWidth();
		vp.Height = graphics.GetScreenHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		dc->RSSetViewports(1, &vp);
		postprocessingRneder->Render(dc);
		postprocessingRneder->DrawDebugGUI();
	}
}

void SceneGame::DebugGui()
{
	if (ImGui::TreeNode("light"))
	{
		ImGui::SliderFloat4("direction_light", &directional_light.x, -1.0f, 1.0f);
		ImGui::ColorEdit3("AmbientColor", &ambientColor.x);
		ImGui::TreePop();
	}
}

void SceneGame::RenderEnemyRockOn(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection
)
{
	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	EnemeyManager& enemyManager = EnemeyManager::Instance();
	int i = player->Get_Rock_num();
	if (!Rock_ON)return;
	Enemy* enemy = enemyManager.GetEnemy(i);
	DirectX::XMVECTOR ScreenPosition;
	DirectX::XMFLOAT3 Position;
	Position = enemy->GetPosition();
	Position.y += enemy->GetHeight();
	ScreenPosition = DirectX::XMVector3Project(
		DirectX::XMLoadFloat3(&Position),
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		0.0f,
		1.0f,
		Projection,
		View,
		World
	);
	DirectX::XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, ScreenPosition);
	sprRock->Render(dc,
		pos.x-50.0f, pos.y - 20.0f,
		100, 100,
		0.0f, 0.0f,
		256.0f, 256.0f,
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
}