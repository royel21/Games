
#include "Layer.h"
#include "Entity.h"
#include "Components.h"

namespace Plutus
{
    Layer::Layer(const std::string &_name) : name(_name)
    {
    }

    Entity Layer::createEntity(const std::string &name)
    {
        Entity ent = {mRegistry.create(), this};
        ent.addComponent<TagComponent>(name.empty() ? "Entity" : name);
        ent.addComponent<TransformComponent>(0.0f, 0.0f);
        return ent;
    }
} // namespace Plutus