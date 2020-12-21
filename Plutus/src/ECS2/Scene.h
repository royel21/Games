#pragma once
#include <unordered_map>
#include <cstring>
#include <memory>

namespace Plutus
{
    class Layer;
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();
        std::unordered_map<std::string, Layer *> *getLayers() { return &mLayers; };
        Entity createEntity(const std::string &name);

    private:
        std::unordered_map<std::string, Layer *> mLayers;
        Layer *currentLayer;
        entt::registry mRegistry;
        friend class Entity;
        friend class Layer;
    };
} // namespace Plutus