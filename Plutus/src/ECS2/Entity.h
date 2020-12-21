#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <cstring>
#include "entt.hpp"
#include "Scene.h"
#include <assert.h>

namespace Plutus
{
    class Entity
    {
    public:
        Entity(entt::entity entity, Scene *layer);

        template <typename T>
        bool hasComponent()
        {
            return mRegistry.has<T>(mEntityId);
        }

        template <typename T, typename... Args>
        T &addComponent(Args &&... args)
        {
            assert(hasComponent<T>() && "Entity already has component!");
            return mRegistry.emplace<T>(mEntityId, std::forward<Args>(args)...);
        }

        template <typename T>
        T &getComponent()
        {
            assert(hasComponent<T>() && "Entity does not has component!");
            return mRegistry.get<T>(mEntityId);
        }

        template <typename T>
        bool removeComponent()
        {
            assert(hasComponent<T>() && "Entity does not has component!");
            return mRegistry.remove<T>(mEntityId);
        }

        operator bool() const { return mEntityId != entt::null; }

    private:
        entt::entity mEntityId{entt::null};
        Layer *mLayer;
    };
} // namespace Plutus
#endif // __ENTITY_H__