
#include "Sound/StringUtil.h"
namespace Plutus
{
	std::string getExtension(const std::string &fileName)
	{
		auto pos = fileName.find_last_of(".");
		if (pos != std::string::npos)
		{
			return fileName.substr(pos + 1);
		}
		return "";
	}
} // namespace Plutus