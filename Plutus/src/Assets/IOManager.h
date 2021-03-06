#ifndef _IOMANAGER_h
#define _IOMANAGER_h

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace Plutus
{
	namespace IOManager
	{
		std::string readFileToString(const std::string &filePath)
		{
			std::ifstream f(filePath);
			if (f.fail())
			{
				f.close();
				return "";
			}
			else
			{
				std::stringstream buffer;
				buffer << f.rdbuf();
				return buffer.str();
			}
		}
	} // namespace IOManager

} // namespace Plutus

#endif