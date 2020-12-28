#pragma once

namespace Plutus
{
    class EntityManager;

    class SceneLoader
    {
    public:
        static bool loadFromJson(const char *path, EntityManager *entManager);
    };
} // namespace Plutus