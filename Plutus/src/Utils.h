#pragma once
#include <string>
#include "rapidjson/document.h"

namespace Plutus
{
    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef unsigned long ulong;
    namespace Utils
    {
        //Get file extenxion
        inline std::string getExtension(const std::string &fileName)
        {
            auto pos = fileName.find_last_of(".");
            if (pos != std::string::npos)
            {
                return fileName.substr(pos + 1);
            }
            return "";
        }

        inline bool loadJson(const std::string &filePath, rapidjson::Document *document)
        {
            std::string ex = getExtension(filePath);
            if (ex == "json")
            {
                auto data = IOManager::readFileToString(filePath);
                if (!data.empty())
                {
                    return document->Parse(data.c_str()).HasParseError() == false;
                }
            }
            return false;
        }
    } // namespace Utils

} // namespace Plutus