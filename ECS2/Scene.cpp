#include "Scene.h"
#include "SDL.h"

#include "Log/Logger.h"
#include "Serialize/Serialize.h"

#include "Graphics/SpriteBatch2D.h"
#include "Graphics/SpriteBatch.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera2D.h"
#include "Graphics/Renderable2D.h"
#include "Graphics/GLSL.h"

#include "ECS/Components/Tag.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/TileMap.h"

namespace Plutus
{
    Scene::Scene()
    {
        mLayers["Layer0"].name = "Layer0";
        mCurrentLayer = &mLayers["Layer0"];
        mRenderer = new SpriteBatch2D();
        mShader = new Shader();
    };

    Scene::~Scene()
    {
        mLayers.clear();
        mShader->dispose();

        delete mRenderer;
        delete mShader;
    }

    void Scene::Init(Camera2D *camera)
    {
        mRenderer->init();
        mCamera = camera;
        mShader->CreateProgWithShader(vertexShader2, fragShader2);
    }

    Entity2 Scene::createEntity(const std::string &name)
    {
        Entity2 ent = {mRegistry.create(), this};
        mCurrentLayer->entities.push_back(ent);
        ent.addComponent<Tag>(name);
        return ent;
    }

    Layer2 *Scene::getLayer(const std::string &name)
    {
        return &mLayers[name];
    }

    Layer2 *Scene::addLayer(const std::string name)
    {
        mLayers[name].name = name;
        mCurrentLayer = &mLayers[name];
        return mCurrentLayer;
    }

    void Scene::draw()
    {
        mShader->enable();
        glClearDepth(1.0f);
        mShader->setUniform1i("hasTexture", 0);
        mShader->setUniform1i("mySampler", 0);
        mShader->setUniform2f("offset", {0, 0});
        mShader->setUniformMat4("camera", mCamera->getCameraMatrix());
        // uint32_t start = SDL_GetTicks();
        for (auto layer : mLayers)
        {
            if (layer.second.isVisible)
            {
                mRenderer->begin(layer.second.entities.size());
                for (auto ent : layer.second.entities)
                {
                    auto trans = mRegistry.get<Transform>(ent.getEntityId());
                    auto sprite = mRegistry.get<Sprite>(ent.getEntityId());
                    sprite.mPosition = trans.position;
                    sprite.mSize = trans.size;
                    mRenderer->submit(&sprite);
                }
                mRenderer->end();
            }
        }
        // LOG_I("time: {0}", SDL_GetTicks() - start);
        // auto group = mRegistry.group<Transform, Sprite>();

        // renderer.begin(group.size());

        // uint32_t start = SDL_GetTicks();
        // for (auto entity : group)
        // {
        //     auto &[trans, sprite] = group.get<Transform, Sprite>(entity);
        //     sprite.mPosition = trans.position;
        //     sprite.mSize = trans.size;
        //     renderer.submit(&sprite);
        // }
        // renderer.end();
        // LOG_I("time: {0}", SDL_GetTicks() - start);

        // renderer2.begin();

        // for (auto entity : group)
        // {
        //     auto &[trans, text] = group.get<Transform, Sprite>(entity);

        //     auto pos = glm::vec4(trans.position.x, trans.position.y, trans.size.x, trans.size.y);

        //     renderer2.draw(pos, text.mUVCoord, text.mColor, text.mTextureId);
        // }
        // renderer2.end();

        mShader->disable();
    }

} // namespace Plutus
