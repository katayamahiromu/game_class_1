#pragma once
#include<memory>
#include<xaudio2.h>
#include<xaudio2fx.h>
#include<xapofx.h>

class SubMixVoice
{
public:
	SubMixVoice(IXAudio2* xaudio);
	~SubMixVoice();
	void SetVolum(float volum);
	void Reverb();
	void echo();
	void equalizer();
private:

public:
	IXAudio2SubmixVoice* Get_Submix_Voice() { return pSubMixVoice; }
private:
	IXAudio2SubmixVoice* pSubMixVoice = nullptr;
};