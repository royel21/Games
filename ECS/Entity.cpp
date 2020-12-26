#include "Entity.h"

namespace Plutus
{
    Entity::Entity(entt::entity Entity, Scene *scene) : mEntityId(Entity), mScene(scene)
    {
    }
} // namespace Plutus