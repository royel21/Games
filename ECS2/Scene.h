#pragma once

#include <memory>
#include <cstring>
#include <entt.hpp>
#include <unordered_map>

namespace Plutus
{
    class Serializer;
    class Scene;
    class Camera2D;
    class SpriteBatch2D;
    class Shader;

    class Entity2
    {
    public:
        Entity2(entt::entity ent, Scene *scene) : mId(ent), mScene(scene) {}

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
        operator entt::entity() const { return mId; }

    private:
        entt::entity mId{entt::null};
        Scene *mScene;
    };

    struct Layer2
    {
        bool isVisible = true;
        std::string name;
        std::vector<Entity2> entities;
    };

    class Scene
    {
    public:
        Scene();
        ~Scene();
        void Init(Camera2D *camera);
        Entity2 createEntity(const std::string &name);

        void setCurrentLayer(const std::string &name) { mCurrentLayer = &mLayers[name]; };
        std::unordered_map<std::string, Layer2> &getLayers() { return mLayers; };

        Layer2 *addLayer(const std::string name);
        Layer2 *getLayer(const std::string &name);
        Layer2 *getCurrentLayer() { return mCurrentLayer; }
        const entt::registry *getRegistry() { return &mRegistry; }

        void draw();

    private:
        std::unordered_map<std::string, Layer2> mLayers;
        Layer2 *mCurrentLayer;
        entt::registry mRegistry;
        Shader *mShader = nullptr;
        SpriteBatch2D *mRenderer = nullptr;
        Camera2D *mCamera = nullptr;
        friend class Entity2;
    };

    template <typename T>
    bool Entity2::hasComponent()
    {
        return mScene->mRegistry.has<T>(mId);
    }

    template <typename T, typename... Args>
    T &Entity2::addComponent(Args &&...args)
    {
        assert(!hasComponent<T>() && "Entity2 already has component!");
        return mScene->mRegistry.emplace<T>(mId, std::forward<Args>(args)...);
    }

    template <typename T>
    T &Entity2::getComponent()
    {
        assert(hasComponent<T>() && "Entity2 does not has component!");
        return mScene->mRegistry.get<T>(mId);
    }

    template <typename T>
    bool Entity2::removeComponent()
    {
        assert(hasComponent<T>() && "Entity2 does not has component!");
        return mScene->mRegistry.remove<T>(mEntityId);
    }
} // namespace Plutus