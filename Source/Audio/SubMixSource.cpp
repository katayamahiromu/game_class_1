#include"Audio/SubMixSource.h"

SubMixVoice::SubMixVoice(IXAudio2* xaudio)
{
	UINT32 Chanels = 2;
	UINT32 SamplingRate = 44100;

	HRESULT hr;
	hr = xaudio->CreateSubmixVoice(&pSubMixVoice, Chanels, SamplingRate);
}

SubMixVoice::~SubMixVoice()
{
	if (pSubMixVoice != nullptr)
	{
		pSubMixVoice->DestroyVoice();
		pSubMixVoice = nullptr;
	}
}

void SubMixVoice::SetVolum(float volum)
{
	pSubMixVoice->SetVolume(volum);
}

void SubMixVoice::Reverb()
{
	IUnknown* pXAPO;
	XAudio2CreateReverb(&pXAPO);


	XAUDIO2_EFFECT_DESCRIPTOR descriptor;
	descriptor.InitialState = true;
	descriptor.OutputChannels = 2;
	descriptor.pEffect = pXAPO;

	XAUDIO2_EFFECT_CHAIN chain;
	chain.EffectCount = 1;
	chain.pEffectDescriptors = &descriptor;

	pSubMixVoice->SetEffectChain(&chain);
	pXAPO->Release();
}

void SubMixVoice::echo()
{
	IUnknown* pXAPO;
	CreateFX(__uuidof(FXEcho), &pXAPO);

	XAUDIO2_EFFECT_DESCRIPTOR descriptor;
	descriptor.InitialState = true;
	descriptor.OutputChannels = 2;
	descriptor.pEffect = pXAPO;

	XAUDIO2_EFFECT_CHAIN chain;
	chain.EffectCount = 1;
	chain.pEffectDescriptors = &descriptor;

	pSubMixVoice->SetEffectChain(&chain);
	pXAPO->Release();
}

void SubMixVoice::equalizer()
{
	IUnknown* pXAPO;
	CreateFX(__uuidof(FXEQ), &pXAPO);

	XAUDIO2_EFFECT_DESCRIPTOR descriptor;
	descriptor.InitialState = true;
	descriptor.OutputChannels = 2;
	descriptor.pEffect = pXAPO;

	XAUDIO2_EFFECT_CHAIN chain;
	chain.EffectCount = 1;
	chain.pEffectDescriptors = &descriptor;

	pSubMixVoice->SetEffectChain(&chain);
	pXAPO->Release();
}