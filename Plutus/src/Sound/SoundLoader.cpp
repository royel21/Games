#include "Sound/SoundLoader.h"
#include "Utils.h"
#include "stb_vorbis.h"
#include <iostream>

namespace Plutus
{

	bool SoundLoader::loadSoundFile(const std::string &filePath, WAVEFORMATEXTENSIBLE *wfx, std::vector<uint8_t> *buffer)
	{
		//#if m_WIN32
		//	_setmode( m_fileno(stdin), m_0_BINARY );
		//#endif

		unsigned long buffSize = 0;
		unsigned long offset = 0;
		if (getExtension(filePath) == "ogg")
		{
			return readOgg(filePath, wfx, buffer);
		}
		else
		{
			bool result = readWave(filePath.c_str(), wfx, buffSize, offset);
			if (result)
			{

				uint8_t *pDataBuffer = new uint8_t[buffSize];
				readChunkToBuffer(filePath, pDataBuffer, buffSize, offset);
				//buffer->reserve(buffSize);
				result = false;
				if (buffSize)
				{
					// Copy the temp buffer to the buffer vector
					buffer->insert(buffer->end(), pDataBuffer, pDataBuffer + buffSize);
					/*for (unsigned long i = 0; i < buffSize; i++) {
						buffer->push_back(pDataBuffer[i]);
					}*/
					result = true;
				}
				delete[] pDataBuffer;

				return result;
			}
		}
		return false;
	}

	bool SoundLoader::readChunkToBuffer(std::string file, void *buffer, unsigned long buffSize, unsigned long offset)
	{
		std::ifstream myfile(file, std::ios::binary);
		if (myfile)
		{
			//move to offset
			myfile.seekg(offset, std::ios::beg);
			//file buffer
			myfile.read((char *)buffer, buffSize);
			myfile.close();
			return true;
		}
		myfile.close();
		return false;
	}

	bool SoundLoader::readWave(std::string filePath, void *wfx, unsigned long &buffSize, unsigned long &offset)
	{
		bool result = true;

		unsigned long chunkType1 = 0;
		unsigned long chunkType2 = 0;
		unsigned long chunkDataSize = 0;

		std::ifstream myfile(filePath, std::ios::binary);

		if (myfile.fail())
			return false;

		myfile.seekg(0, std::ios::beg);

		myfile.read((char *)&chunkType1, 4);
		myfile.seekg(12, std::ios::beg);

		// get chunck size
		myfile.read((char *)&chunkType2, 4);

		if (chunkType1 != 'FFIR' || chunkType2 != ' tmf')
		{
			myfile.close();
			return false;
		}
		//get the wave format size
		myfile.read((char *)&chunkDataSize, 4);
		/*if (chunkDataSize > sizeof(WAVEFORMATEX))
			chunkDataSize = sizeof(WAVEFORMATEX);*/
		////get the WAVE format
		myfile.read((char *)wfx, chunkDataSize);

		bool found = true;
		//loop until data position is found
		char temp;
		unsigned long pos;
		while (found)
		{
			myfile.read(&temp, 1);
			if (temp == 'a')
			{
				pos = (unsigned long)myfile.tellg();
				myfile.seekg(pos - 2, std::ios::beg);
				myfile.read((char *)&chunkType1, 4);
				if (chunkType1 == 'atad')
				{
					found = false;
					break;
				}
			}
		}
		// if the data position is found pass it to the offest with the size
		if (!found)
		{
			myfile.read((char *)&chunkDataSize, 4);
			offset = (unsigned long)myfile.tellg();
			buffSize = chunkDataSize;
		}

		myfile.close();
		return result;
	}

	bool SoundLoader::readOgg(std::string filePath, void *wfx, std::vector<uint8_t> *buffer)
	{
		int error;
		stb_vorbis *_file = stb_vorbis_open_filename(filePath.c_str(), &error, NULL);
		if (!_file)
			std::cout << "Couldn't open " << filePath << " \n";

		stb_vorbis_info v_info = stb_vorbis_get_info(_file);

		WAVEFORMATEX wfm;
		// Set the wave format
		memset(&wfm, 0, sizeof(wfm));

		wfm.cbSize = sizeof(wfm);
		wfm.nChannels = v_info.channels;
		//Ogg vorbis is always 16 bit
		wfm.wBitsPerSample = 16;
		wfm.nSamplesPerSec = v_info.sample_rate;
		wfm.nAvgBytesPerSec = v_info.sample_rate * wfm.nChannels * 2;
		wfm.nBlockAlign = 2 * wfm.nChannels;
		wfm.wFormatTag = WAVE_FORMAT_PCM;
		memcpy(wfx, &wfm, sizeof(wfm));

		const unsigned int _length_samples = (stb_vorbis_stream_length_in_samples(_file) * v_info.channels);

		uint32_t buffSize = _length_samples * 2;
		char *tbuffer = new char[buffSize];

		stb_vorbis_get_samples_short_interleaved(_file, v_info.channels, (short *)tbuffer, _length_samples);

		buffer->insert(buffer->end(), tbuffer, tbuffer + buffSize);
		/*for (unsigned long i = 0; i < buffSize; i++) {
			buffer->push_back((u8)tbuffer[i]);
		}*/

		delete[] tbuffer;

		stb_vorbis_close(_file);

		return true;
	}
} // namespace Plutus