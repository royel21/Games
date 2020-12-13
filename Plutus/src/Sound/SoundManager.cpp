#include "Sound/SoundManager.h"
#include <iostream>
namespace Plutus
{
	bool SoundManager::has_COINIT = false;

	SoundManager::SoundManager() : mXAuido2(NULL), mMasterVoice(NULL)
	{
		Init();
	}

	SoundManager *SoundManager::getManager()
	{
		has_COINIT = true;
		static SoundManager *Instance = new SoundManager();
		return Instance;
	}

	SoundManager::~SoundManager()
	{
		std::cout << "SM Destructor\n";

		CleanUp();

		if (mMasterVoice)
		{
			mMasterVoice->DestroyVoice();
			std::cout << "XAudio engine release\n";
		}
		if (mXAuido2)
		{
			mXAuido2->StopEngine();
			mXAuido2->Release();
			std::cout << "XAudio engine release\n";
		}
		if (has_COINIT)
			CoUninitialize();
	}
	bool SoundManager::Init()
	{
		if (has_COINIT)
		{
			if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
				return 0;
		}

		HRESULT hr = S_OK;

		hr = XAudio2Create(&mXAuido2);
		if (FAILED(hr))
		{
			std::cout << "Critical error: Unable to create the XAudio2 engine!\n";
			return false;
		}

		hr = mXAuido2->CreateMasteringVoice(&mMasterVoice);
		if (FAILED(hr))
		{
			std::cout << "Critical error: Unable to create the XAudio2 mastering voice!\n";
			return false;
		}
		mXAuido2->CreateSubmixVoice(&mSoundsSubmix, 1, 44100, 0, 0, 0, 0);
		mXAuido2->CreateSubmixVoice(&mMusicSubmix, 1, 44100, 0, 0, 0, 0);

		sendSounds = {0, mSoundsSubmix};
		sendMusic = {0, mMusicSubmix};

		mSoundsSendList = {1, &sendSounds};
		mMusicSendList = {1, &sendMusic};

		std::cout << "Xaudio2 Engine Initialize\n";

		return true;
	}

	uint32_t SoundManager::addAudio(std::string name, std::string path, SoundType type)
	{
		if (!mXAuido2 || !mMasterVoice || mAudioEvents[name])
			return false;

		auto event = new AudioEvent(name, path);

		if (!event->mAudioData.size())
		{
			std::cout << "Critical error: can't load file!\n";
			return 1;
		}
		event->type = type;
		mAudioEvents[name] = event;
		HRESULT hr = S_OK;
		// create source voice
		if (type == MUSIC)
		{

			hr = mXAuido2->CreateSourceVoice(
				&event->mSourceVoice,
				&event->mWaveFormat.Format,
				0, XAUDIO2_DEFAULT_FREQ_RATIO,
				nullptr,
				&mSoundsSendList,
				NULL);
		}
		else
		{
			hr = mXAuido2->CreateSourceVoice(
				&event->mSourceVoice,
				&event->mWaveFormat.Format,
				0, XAUDIO2_DEFAULT_FREQ_RATIO,
				nullptr,
				&mMusicSendList,
				NULL);
		}
		if (FAILED(hr))
		{
			std::cout << "Critical error: Unable to create source voice!\n";
			return 1;
		}
		// return success
		return 0;
	}

	bool SoundManager::PlayAudio(std::string name, bool loop)
	{
		auto aEvent = mAudioEvents[name];
		if (aEvent)
		{
			XAUDIO2_VOICE_STATE aState;
			aEvent->mSourceVoice->GetState(&aState);

			if (aState.BuffersQueued)
			{
				aEvent->mSourceVoice->Stop();
				aEvent->mSourceVoice->FlushSourceBuffers();
			}

			aEvent->mAudioBuffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

			if (aEvent)
				return aEvent->start();
		}
		return false;
	}

	bool SoundManager::Pause(std::string name)
	{
		auto aEvent = mAudioEvents[name];
		if (aEvent)
		{
			return aEvent->pause();
		}

		return false;
	}

	bool SoundManager::Stop(std::string name)
	{
		auto aEvent = mAudioEvents[name];
		if (aEvent)
		{
			return aEvent->stop();
		}

		return false;
	}

	void SoundManager::removeAudio(std::string name)
	{
		auto &aEvent = mAudioEvents[name];
		if (aEvent)
		{
			aEvent->mSourceVoice->Stop();
			delete aEvent;
			mAudioEvents.erase(name);
		}
	}

	void SoundManager::CleanUp()
	{
		for (auto &itr : mAudioEvents)
		{
			std::cout << "Deleting AudioEvent \n";
			delete itr.second;
		}
		mAudioEvents.clear();
	}
} // namespace Plutus