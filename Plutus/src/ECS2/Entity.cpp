#include "Entity.h"

namespace Plutus
{
    Entity::Entity(entt::entity entity, Layer *layer)
        : mEntityId(entity), mLayer(layer)
    {
    }
} // namespace Plutus