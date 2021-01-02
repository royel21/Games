#pragma once
#include <string>
#include "rapidjson/document.h"
#include "Assets/IOManager.h"
#include "glm/glm.hpp"

#define SAVE_FILE 0
#define OPEN_FILE 1

#define LIMIT(v, min, max) v<min ? min : v> max ? max : v

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

        inline bool loadJson(const char *filePath, rapidjson::Document *document)
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

        bool windowDialog(int mode, std::string &path);

        void toJsonFile(std::string filepath, const char *buffer);
    } // namespace Utils

} // namespace Plutus