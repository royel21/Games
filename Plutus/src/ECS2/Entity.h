#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <cstring>
#include "entt.hpp"
#include "Layer.h"
#include <assert.h>

namespace Plutus
{
    class Entity
    {
    public:
        Entity(entt::entity entity, Layer *layer);

        template <typename T>
        bool hasComponent()
        {
            return mLayer->mRegistry.has<T>(mEntityId);
        }

        template <typename T, typename... Args>
        T &addComponent(Args &&... args)
        {
            assert(hasComponent<T>() && "Entity already has component!");
            return mLayer->mRegistry.emplace<T>(mEntityId, std::forward<Args>(args)...);
        }

        template <typename T>
        T &getComponent()
        {
            assert(hasComponent<T>() && "Entity does not has component!");
            return mLayer->mRegistry.get<T>(mEntityId);
        }

        template <typename T>
        bool removeComponent()
        {
            assert(hasComponent<T>() && "Entity does not has component!");
            return mLayer->mRegistry.remove<T>(mEntityId);
        }

        operator bool() const { return mEntityId != entt::null; }

    private:
        entt::entity mEntityId{entt::null};
        Layer *mLayer;
    };
} // namespace Plutus
#endif // __ENTITY_H__