#pragma once

#include <xaudio2.h>
#include "Audio/AudioSource.h"
#include"Audio/SubMixSource.h"

// オーディオ
class Audio
{
public:
	Audio();
	~Audio();

public:
	// インスタンス取得
	static Audio& Instance() { return *instance; }

	// オーディオソース読み込み
	std::unique_ptr<AudioSource> LoadAudioSource(const char* filename, IXAudio2SubmixVoice* sv = nullptr);
	//サブミックスボイスの作成
	std::unique_ptr<SubMixVoice> MakeSubMix();
public:
	IXAudio2* Get_IXAudio2() { return xaudio; }
private:
	static Audio*			instance;

	IXAudio2*				xaudio = nullptr;
	IXAudio2MasteringVoice* masteringVoice = nullptr;
};
