#include "Scene.h"
#include "SDL.h"

#include "Log/Logger.h"
#include "Graphics/GLSL.h"
#include "Graphics/Renderable2D.h"
#include "Serialize/Serialize.h"
#include "Log/Logger.h"

#include "Components.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"

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
        // auto view = mRegistry.view<Transform>();

        // for (auto entity : view)
        // {
        //     // a  at a time ...
        //     auto &trans = view.get<Transform>(entity);
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
        mCurrentLayer->entities.push_back(ent);
        ent.addComponent<Tag>(name);
        return ent;
    }

    Layer *Scene::getLayer(const std::string &name)
    {
        return &mLayers[name];
    }

    Layer *Scene::addLayer(const std::string name)
    {
        mLayers[name].name = name;
        mCurrentLayer = &mLayers[name];
        return mCurrentLayer;
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
            if (layer.second.isVisible)
            {
                renderer.begin(layer.second.entities.size());
                for (auto ent : layer.second.entities)
                {
                    auto trans = mRegistry.get<Transform>(ent.getEntityId());
                    auto sprite = mRegistry.get<Sprite>(ent.getEntityId());
                    sprite.mPosition = trans.position;
                    sprite.mSize = trans.size;
                    renderer.submit(&sprite);
                }
                renderer.end();
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

        mShader.disable();
    }

    void Scene::Serialize(Serializer &ser)
    {
        auto writer = ser.getWriter();
        writer->StartObject();

        writer->String("layers");
        writer->StartArray();
        for (auto layer : mLayers)
        {
            writer->StartObject();
            {
                writer->String("name");
                writer->String(layer.first.c_str());
                writer->String("entities");
                writer->StartArray();
                {
                    writer->StartObject();
                    for (auto ent : layer.second.entities)
                    {
                        ent.getComponent<Tag>().Serialize(ser);
                        if (ent.hasComponent<Transform>())
                        {
                            ent.getComponent<Transform>().Serialize(ser);
                        }
                        if (ent.hasComponent<Animate>())
                        {
                            ent.getComponent<Animate>().Serialize(ser);
                        }
                        if (ent.hasComponent<Sprite>())
                        {
                            ent.getComponent<Sprite>().Serialize(ser);
                        }
                        if (ent.hasComponent<TileMap>())
                        {
                            ent.getComponent<TileMap>().Serialize(ser);
                        }
                    }
                    writer->EndObject();
                }
                writer->EndArray();
            }
            writer->EndObject();
        }
        writer->EndArray();

        writer->EndObject();
    }
} // namespace Plutus

//
// writer->String("texId");
// writer->Int(tile.x);
// writer->StartArray();
// writer->EndArray();
//