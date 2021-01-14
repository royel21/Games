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
} // namespace Plutus