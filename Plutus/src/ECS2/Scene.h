#pragma once
#include <unordered_map>
#include <cstring>
#include <memory>

namespace Plutus
{
    class Layer;

    class Scene
    {
    public:
        Scene();
        ~Scene();
        std::unordered_map<std::string, Layer *> *getLayers() { return &mLayers; };

    private:
        std::unordered_map<std::string, Layer *> mLayers;
        Layer *currentLayer;
    };
} // namespace Plutus