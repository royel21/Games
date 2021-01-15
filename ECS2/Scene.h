#pragma once

#include <unordered_map>
#include <cstring>
#include <memory>
#include "entt.hpp"
#include "Graphics/SpriteBatch2D.h"
#include "Graphics/SpriteBatch.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera2D.h"

namespace Plutus
{
    class Serializer;

    class Entity
    {
    public:
        Entity(entt::entity ent, Scene *scene) : mId(ent), mScene(scene) {}

        template <typename T>
        bool hasComponent();

        template <typename T, typename... Args>
        T &addComponent(Args &&...args);

        template <typename T>
        T &getComponent();

        template <typename T>
        bool removeComponent();

        entt::entity getEntityId() const { return mId; }
        operator bool() const { return mId != entt::null; }
        operator entt::entity &() { return &mId }

    private:
        entt::entity mId{entt::null};
        Scene *mScene;
    };

    struct Layer
    {
        bool isVisible = true;
        std::string name;
        std::vector<Entity> entities;
    };

    class Scene
    {
    public:
        Scene();
        ~Scene();
        void Init(Camera2D *camera);
        void update();
        Entity createEntity(const std::string &name);

        void setCurrentLayer(const std::string &name) { mCurrentLayer = &mLayers[name]; };
        std::unordered_map<std::string, Layer> &getLayers() { return mLayers; };

        Layer *addLayer(const std::string name);
        Layer *getLayer(const std::string &name);
        Layer *getCurrentLayer() { return mCurrentLayer; }
        const entt::registry *getRegistry() { return &mRegistry; }

        void Serialize(Serializer &serializer);
        void draw();

    private:
        std::unordered_map<std::string, Layer> mLayers;
        Layer *mCurrentLayer;
        entt::registry mRegistry;
        SpriteBatch2D renderer;
        SpriteBatch renderer2;
        Shader mShader;
        Camera2D *mCamera;
        friend class Entity;
    };

    template <typename T>
    bool Entity::hasComponent()
    {
        return mScene->mRegistry.has<T>(mId);
    }

    template <typename T, typename... Args>
    T Entity::&addComponent(Args &&...args)
    {
        assert(!hasComponent<T>() && "Entity already has component!");
        return mScene->mRegistry.emplace<T>(mId, std::forward<Args>(args)...);
    }

    template <typename T>
    T Entity::&getComponent()
    {
        assert(hasComponent<T>() && "Entity does not has component!");
        return mScene->mRegistry.get<T>(mId);
    }

    template <typename T>
    bool Entity::removeComponent()
    {
        assert(hasComponent<T>() && "Entity does not has component!");
        return mScene->mRegistry.remove<T>(mEntityId);
    }
} // namespace Plutus