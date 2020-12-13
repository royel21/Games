#pragma once

#include <xaudio2.h>
#include <vector>
#include <string>
namespace Plutus {
	class SoundManager;

	class AudioEvent {
	private:
		std::string mName;
		// the XAudio2 source voice
		IXAudio2SourceVoice* mSourceVoice;
		// the length of the wave
		unsigned int mWaveLength;
		// the audio data
		std::vector<uint8_t> mAudioData;
		// the actual buffer with the audio data
		XAUDIO2_BUFFER mAudioBuffer;
		// falloff distance
		float mFallof;
		// music priority
		unsigned int mPriority;
		// the index of the actual sound to play
		unsigned int mIndex;
		//Audio Type
		int type;
		// the format of the audio file
		WAVEFORMATEXTENSIBLE mWaveFormat;

		friend class SoundManager;
	public:
		AudioEvent(std::string name, std::string filePath);

		~AudioEvent();

		bool start();

		bool stop();

		bool pause();
	};
}