#pragma once

#include <xaudio2.h>
#include <iostream>
#include <fstream>
#include <vector>
namespace Plutus {
	class SoundLoader
	{
	public:
		static bool loadSoundFile(const std::string& filePath, WAVEFORMATEXTENSIBLE* wfx, std::vector<uint8_t>* buffer);

	private:
		static bool readChunkToBuffer(std::string file, void* buffer, unsigned long buffSize, unsigned long offset);
		static bool readWave(std::string filePath, void* wfx, unsigned long& buffSize, unsigned long& offset);
		static bool readOgg(std::string filePath, void* wfx, std::vector<uint8_t>* buffer);
	};
}