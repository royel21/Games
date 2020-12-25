#pragma once
#include <string>

namespace Plutus
{
    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef unsigned long ulong;

    //Get file extenxion
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