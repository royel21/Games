#pragma once

#include "Scene.h"

namespace Plutus
{
    class Entity
    {
    public:
        Entity(entt::entity Entity, Scene *scence);

        template <typename T>
        bool hasComponent()
        {
            return mScene->mRegistry.has<T>(mEntityId);
        }

        template <typename T, typename... Args>
        T &addComponent(Args &&... args)
        {
            assert(!hasComponent<T>() && "Entity already has component!");
            return mScene->mRegistry.emplace<T>(mEntityId, std::forward<Args>(args)...);
        }

        template <typename T>
        T &getComponent()
        {
            assert(hasComponent<T>() && "Entity does not has component!");
            return mScene->mRegistry.get<T>(mEntityId);
        }

        template <typename T>
        bool removeComponent()
        {
            assert(hasComponent<T>() && "Entity does not has component!");
            return mScene->mRegistry.remove<T>(mEntityId);
        }
        entt::entity getEntityId() { return mEntityId; }
        operator bool() const { return mEntityId != entt::null; }

    private:
        entt::entity mEntityId{entt::null};
        Scene *mScene;
    };
} // namespace Plutus