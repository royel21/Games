#include "SceneLoader.h"
#include <string>

#include "EntityManager.h"
#include "rapidjson/document.h"
#include "Utils.h"
#include "Components/Animate.h"
#include "Components/Transform.h"
#include "Components/Sprite.h"
#include "Components/TileMap.h"

namespace Plutus
{
    void loadAnimation(Entity *ent, rapidjson::Value::Object &value)
    {
    }

    void loadSprite(Entity *ent, rapidjson::Value::Object &value)
    {
    }
    void loadTileMap(Entity *ent, rapidjson::Value::Object &value)
    {
    }

    bool SceneLoader::loadFromJson(const char *path, EntityManager *entManager)
    {
        bool success = false;
        rapidjson::Document doc;
        if (Utils::loadJson(path, &doc))
        {
            if (doc["layers"].IsArray())
            {
                entManager->clearData();
                //Get the layers
                auto layers = doc["layers"].GetArray();
                for (size_t i = 0; i < layers.Size(); i++)
                {
                    auto &objLayer = layers[i].GetJsonObject();

                    auto layerName = objLayer["name"].GetString();
                    auto layer = entManager->addLayer(layerName);

                    //get the entities
                    auto entities = objLayer["entities"].GetArray();
                    for (size_t i = 0; i < entities.Size(); i++)
                    {
                        auto &objEntity = entities[i].GetJsonObject();
                        auto entityName = objEntity["name"].GetString();
                        auto entity = entManager->addEntity(entityName);

                        auto components = objEntity["components"].GetArray();
                        for (size_t i = 0; i < components.Size(); i++)
                        {
                            auto component = components[i].GetJsonObject();
                            std::string compType = component["name"].GetString();
                            if (compType == "Transform")
                            {
                                int x = component["x"].GetInt();
                                int y = component["y"].GetInt();
                                int w = component["w"].GetInt();
                                int h = component["h"].GetInt();
                                entity->addComponent<Transform>(x, y, h, w);
                                continue;
                            }
                            if (compType == "Sprite")
                            {
                                entity->addComponent<Sprite>(component["texture"].GetString());
                                continue;
                            }
                            if (compType == "Animation")
                            {
                                loadAnimation(entity, components[i].GetJsonObject());
                                continue;
                            }
                            if (compType == "TileMap")
                            {
                                loadTileMap(entity, components[i].GetJsonObject());
                            }
                        }
                    }
                }
                success = true;
            }
        }
        return success;
    }
} // namespace Plutus
