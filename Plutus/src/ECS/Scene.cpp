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
    };

    Scene::~Scene()
    {
        mLayers.clear();
    }

    void Scene::Init(int width, int height)
    {
        renderer.init();
        renderer2.init();
        mCamera.init(width, height);
        mShader.CreateProgWithShader(vertexShader2, fragShader2);
    }

    void Scene::update()
    {
        mCamera.update();
    }

    Entity Scene::createEntity(const std::string &name)
    {
        Entity ent = {mRegistry.create(), this};
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
        mShader.setUniformMat4("camera", mCamera.getCameraMatrix());

        auto group = mRegistry.group<TransformComponent, SpriteComponent>();

        uint32_t start = SDL_GetTicks();
        renderer.begin(group.size());

        for (auto entity : group)
        {
            auto &[trans, sprite] = group.get<TransformComponent, SpriteComponent>(entity);

            sprite.mPosition = trans.position;
            sprite.mSize = trans.size;

            renderer.submit(&sprite);
        }
        renderer.end();
        LOG_I("time: {0}", SDL_GetTicks() - start);

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