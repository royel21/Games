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
