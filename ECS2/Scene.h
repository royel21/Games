#pragma once

#include <unordered_map>
#include <cstring>
#include <memory>
#include "entt.hpp"
#include "Graphics/SpriteBatch2D.h"
#include "Graphics/SpriteBatch.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera2D.h"
#include "Layer.h"

namespace Plutus
{
    class Serializer;
    class Entity;
    class Layer;

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
    };

    class Entity
    {
    public:
        Entity(entt::entity entity, Scene *scene) : id(entity), mScene(scene) {}

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
        entt::entity id{entt::null};
        Scene *mScene;
    };

    struct Layer
    {
        bool isVisible = true;
        std::string name;
        std::vector<Entity> entities;
    };
} // namespace Plutus