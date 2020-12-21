#include "Scene.h"
#include "Layer.h"
#include "Entity.h"
#include "Components.h"

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

    Entity Scene::createEntity(const std::string &name)
    {
        Entity ent = {mRegistry.create(), this};
        ent.addComponent<TagComponent>(name.empty() ? "Entity" : name);
        ent.addComponent<TransformComponent>(0.0f, 0.0f);
        return ent;
    }
} // namespace Plutus