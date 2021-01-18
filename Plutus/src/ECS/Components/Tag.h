#pragma once
#include <string>

namespace Plutus
{
    struct Tag
    {
        std::string Name;
        Tag(const std::string name) : Name(name) {}
    };
} // namespace Plutus