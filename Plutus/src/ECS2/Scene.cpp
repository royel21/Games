#include "Scene.h"
#include "Layer.h"

namespace Plutus
{
    Scene::Scene()
    {
        mLayers.emplace("Layer0");
    };

    Scene::~Scene()
    {
        for (auto &l : mLayers)
        {
            delete l.second;
        }
        mLayers.clear();
    }
} // namespace Plutus