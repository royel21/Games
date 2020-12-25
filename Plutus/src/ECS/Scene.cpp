#include "Scene.h"
#include "Entity.h"
#include "Log/Logger.h"
#include "Graphics/GLSL.h"
#include "Graphics/Renderable2D.h"
#include "Log/Logger.h"

#include "Components.h"
#include "SpriteComponent.h"
#include "SDL.h"

namespace Plutus
{
    Scene::Scene()
    {
        mLayers["Layer0"].name = "Layer0";
        mCurrentLayer = &mLayers["Layer0"];
    };

    Scene::~Scene()
    {
        mLayers.clear();
    }

    void Scene::Init(Camera2D *camera)
    {
        renderer.init();
        renderer2.init();
        mCamera = camera;
        mShader.CreateProgWithShader(vertexShader2, fragShader2);
    }

    void Scene::update()
    {
        // auto view = mRegistry.view<TransformComponent>();

        // for (auto entity : view)
        // {
        //     // a component at a time ...
        //     auto &trans = view.get<TransformComponent>(entity);
        //     trans.position += 2;
        //     if (trans.position.x > mCamera.getScaleScreen().x / 2)
        //     {
        //         trans.position.x = mCamera.getScaleScreen().x / 2 * -1;
        //         trans.position.y = mCamera.getScaleScreen().y / 2 * -1;
        //     }
        // }
    }

    Entity Scene::createEntity(const std::string &name)
    {
        Entity ent = {mRegistry.create(), this};
        mCurrentLayer->entities.push_back(ent.getEntityId());
        ent.addComponent<TagComponent>(name);
        return ent;
    }

    Layer2 *Scene::getLayer(const std::string &name)
    {
        return &mLayers[name];
    }

    void Scene::draw()
    {
        mShader.enable();
        glClearDepth(1.0f);
        mShader.setUniform1i("hasTexture", 0);
        mShader.setUniform1i("mySampler", 0);
        mShader.setUniformMat4("camera", mCamera->getCameraMatrix());
        // uint32_t start = SDL_GetTicks();
        for (auto layer : mLayers)
        {
            renderer.begin(layer.second.entities.size());
            for (auto ent : layer.second.entities)
            {
                auto trans = mRegistry.get<TransformComponent>(ent);
                auto sprite = mRegistry.get<SpriteComponent>(ent);
                sprite.mPosition = trans.position;
                sprite.mSize = trans.size;
                renderer.submit(&sprite);
            }
            renderer.end();
        }
        // LOG_I("time: {0}", SDL_GetTicks() - start);
        // auto group = mRegistry.group<TransformComponent, SpriteComponent>();

        // renderer.begin(group.size());

        // uint32_t start = SDL_GetTicks();
        // for (auto entity : group)
        // {
        //     auto &[trans, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
        //     sprite.mPosition = trans.position;
        //     sprite.mSize = trans.size;
        //     renderer.submit(&sprite);
        // }
        // renderer.end();
        // LOG_I("time: {0}", SDL_GetTicks() - start);

        // renderer2.begin();

        // for (auto entity : group)
        // {
        //     auto &[trans, text] = group.get<TransformComponent, SpriteComponent>(entity);

        //     auto pos = glm::vec4(trans.position.x, trans.position.y, trans.size.x, trans.size.y);

        //     renderer2.draw(pos, text.mUVCoord, text.mColor, text.mTextureId);
        // }
        // renderer2.end();

        mShader.disable();
    }

    void Scene::addLayer(const std::string &name)
    {
        mLayers[name].name = name;
    }
} // namespace Plutus