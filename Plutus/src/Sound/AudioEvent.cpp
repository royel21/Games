#include "Sound/AudioEvent.h"
#include "Sound/SoundLoader.h"

#include <iostream>
namespace Plutus
{
	AudioEvent::AudioEvent(std::string name, std::string filePath) : mSourceVoice(nullptr),
																	 mWaveLength(0),
																	 mFallof(0.0f),
																	 mPriority(0),
																	 mIndex(0)
	{
		mName = name;
		mAudioData.clear();

		std::cout << "Adding new Audio Event\n";
		bool result = SoundLoader::loadSoundFile(filePath, &mWaveFormat, &mAudioData);
		if (result)
		{
			ZeroMemory(&mAudioBuffer, sizeof(XAUDIO2_BUFFER));
			mAudioBuffer.AudioBytes = (UINT32)mAudioData.size();
			mAudioBuffer.pAudioData = (BYTE *const) & mAudioData[0];
			mAudioBuffer.pContext = nullptr;
		}
	}

	AudioEvent::~AudioEvent()
	{
		mAudioData.clear();
		ZeroMemory(&mAudioBuffer, sizeof(XAUDIO2_BUFFER));

		// delete the source voice
		if (mSourceVoice)
		{
			mSourceVoice->Stop();
			mSourceVoice->DestroyVoice();
		}
		std::cout << "AudioEvent Destructor called\n";
	}

	bool AudioEvent::start()
	{
		HRESULT hr = S_OK;

		// submit the audio buffer to the source voice
		hr = mSourceVoice->SubmitSourceBuffer(&mAudioBuffer);
		if (FAILED(hr))
		{
			std::cout << "Critical error: Unable to submit source buffer!\n";
			return false;
		}

		// start the source voice
		mSourceVoice->Start();

		return true;
	}

	bool AudioEvent::pause()
	{
		mSourceVoice->Stop();
		return false;
	}

	bool AudioEvent::stop()
	{
		if (type == 1)
		{
		}
		else
		{
		}
		mAudioBuffer.LoopCount = 0;
		mSourceVoice->Stop();
		mSourceVoice->FlushSourceBuffers();
		mSourceVoice->SubmitSourceBuffer(&mAudioBuffer);
		return true;
	}
} // namespace Plutus