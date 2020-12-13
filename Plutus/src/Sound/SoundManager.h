#pragma once

#include <map>
#include <string>

#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")

#include "AudioEvent.h"

namespace Plutus
{
	enum SoundType
	{
		MUSIC,
		EFFECT
	};

	class SoundManager
	{
	private:
		IXAudio2 *mXAuido2;
		IXAudio2MasteringVoice *mMasterVoice;
		// submix voices
		IXAudio2SubmixVoice *mSoundsSubmix; // collection of all sound effects
		IXAudio2SubmixVoice *mMusicSubmix;	// collection of all music files

		XAUDIO2_SEND_DESCRIPTOR sendSounds;

		XAUDIO2_SEND_DESCRIPTOR sendMusic;

		XAUDIO2_VOICE_SENDS mSoundsSendList;
		XAUDIO2_VOICE_SENDS mMusicSendList;

		std::map<std::string, AudioEvent *> mAudioEvents;

	public:
		static SoundManager *getManager();

		~SoundManager();

		uint32_t addAudio(std::string name, std::string path, SoundType type);

		bool PlayAudio(std::string name, bool loop = false);

		bool Pause(std::string name);

		bool Stop(std::string name);

		void removeAudio(std::string name);

	private:
		static bool has_COINIT;
		SoundManager();
		bool Init();

		void CleanUp();
	};

} // namespace Plutus