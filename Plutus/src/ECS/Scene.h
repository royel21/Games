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
    class Entity;

    struct Layer2
    {
        bool isVisible = true;
        std::string name;
        std::vector<entt::entity> entities;
    };

    class Scene
    {
    public:
        Scene();
        ~Scene();
        void Init(Camera2D *camera);
        void update();
        Entity createEntity(const std::string &name);

        void addLayer(const std::string &name);
        Layer2 *getLayer(const std::string &name);
        std::unordered_map<std::string, Layer2> *getLayers() { return &mLayers; };

        void draw();

    private:
        std::unordered_map<std::string, Layer2> mLayers;
        Layer2 *mCurrentLayer;
        entt::registry mRegistry;
        SpriteBatch2D renderer;
        SpriteBatch renderer2;
        Shader mShader;
        Camera2D *mCamera;

        friend class Entity;
    };
} // namespace Plutus