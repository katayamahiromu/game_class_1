#pragma once

#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include "Graphics/Shader.h"
#include "Graphics/DebugRenderer.h"
#include "Graphics/LineRenderer.h"
#include "Graphics/ImGuiRenderer.h"
#include<mutex>

enum class ShadrId
{
	STAGE,
	MODEL,
	Max
};

enum class SpriteShaderId
{
	Default,
	LuminanceExtraction,
	ColorGrading,
	Finalpass,
	Max
};

// �O���t�B�b�N�X
class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();

	// �C���X�^���X�擾
	static Graphics& Instance() { return *instance; }

	// �f�o�C�X�擾
	ID3D11Device* GetDevice() const { return device.Get(); }

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* GetDeviceContext() const { return immediateContext.Get(); }

	// �X���b�v�`�F�[���擾
	IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }

	// �����_�[�^�[�Q�b�g�r���[�擾
	ID3D11RenderTargetView* GetRenderTargetView() const { return renderTargetView.Get(); }

	// �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* GetDepthStencilView() const { return depthStencilView.Get(); }

	//���X�^���C�U�[�擾
	ID3D11RasterizerState* GetRasterizerState() const { return rasterizerstate.Get(); }

	//�u�����h�X�e�[�g�̎擾
	ID3D11BlendState* GetBlendState()const { return blend_state.Get(); }

	// �V�F�[�_�[�擾
	Shader* GetShader(ShadrId id) const { return shader[static_cast<int>(id)].get(); }

	// �X�v���C�g�V�F�[�_�[�擾
	SpriteShader* GetShader(SpriteShaderId id) const { return spriteShaders[static_cast<int>(id)].get(); }

	// �X�N���[�����擾
	float GetScreenWidth() const { return screenWidth; }

	// �X�N���[�������擾
	float GetScreenHeight() const { return screenHeight; }

	// �f�o�b�O�����_���擾
	DebugRenderer* GetDebugRenderer() const { return debugRenderer.get(); }

	// ���C�������_���擾
	LineRenderer* GetLineRenderer() const { return lineRenderer.get(); }

	// ImGui�����_���擾
	ImGuiRenderer* GetImGuiRenderer() const { return imguiRenderer.get(); }

	//�~���[�e�b�N�X�擾
	std::mutex& GetMutex() { return mutex; }
private:
	static Graphics*								instance;

	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerstate;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_state;

	std::unique_ptr<Shader>							shader[static_cast<int>(ShadrId::Max)];
	std::unique_ptr<DebugRenderer>					debugRenderer;
	std::unique_ptr<LineRenderer>					lineRenderer;
	std::unique_ptr<ImGuiRenderer>					imguiRenderer;
	std::unique_ptr<SpriteShader>					spriteShaders[static_cast<int>(SpriteShaderId::Max)];

	float	screenWidth;
	float	screenHeight;
	std::mutex mutex;
};

