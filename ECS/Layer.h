#pragma once
#include <vector>

namespace Plutus
{
    class Entity;

    struct Layer
    {
        bool isVisible = true;
        std::string name;
        std::vector<Entity> entities;
    };
} // namespace Plutus