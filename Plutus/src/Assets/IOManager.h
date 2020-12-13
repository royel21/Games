#ifndef _IOMANAGER_h
#define _IOMANAGER_h

#include <vector>
#include <string>
#include <fstream>
namespace Plutus {
	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {
			//Read a file as binary data
			std::ifstream file(filePath, std::ios::binary);

			if (file.fail()) {
				perror(filePath.c_str());
				return false;
			}

			//seek to the end of the file
			file.seekg(0, std::ios::end);

			//get the file size
			std::streamoff fileSize = file.tellg();

			//Return to the begining of the file
			file.seekg(0, std::ios::beg);

			//Reduce the file size by any header that might be present
			fileSize -= file.tellg();

			//Make the buffer of file size
			buffer.resize((const unsigned int)fileSize);

			//Read the file into the buffer
			file.read((char*) & (buffer[0]), fileSize);

			file.close();


			return true;

		}
	};

}


#endif