#include "SceneLoader.h"
#include <string>

#include "EntityManager.h"
#include "rapidjson/document.h"
#include "Utils.h"
#include "Components/Animate.h"
#include "Components/Transform.h"
#include "Components/Sprite.h"
#include "Components/TileMap.h"
#include "SDL.h"

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
        int w = value["tilewidth"].GetInt();
        int h = value["tileheight"].GetInt();
        auto tileset = value["tileset"].GetString();
        auto &tmap = ent->addComponent<TileMap>(w, h);
        tmap.mTileset = AssetManager::getInstance()->getTexture(tileset);

        auto tiles = value["tiles"].GetArray();
        std::cout << tiles.Size() << std::endl;
        for (size_t i = 0; i < tiles.Size(); i++)
        {
            auto tile = tiles[i].GetJsonObject();
            int x = tile["x"].GetInt();
            int y = tile["y"].GetInt();
            int texId = tile["texId"].GetInt();
            bool isSolid = tile["isSolid"].GetBool();
            tmap.mTiles.emplace_back(x, y, texId, isSolid);
        }
    }

    bool SceneLoader::loadFromJson(const char *path, EntityManager *entManager)
    {
        uint32_t start = SDL_GetTicks();
        bool success = false;
        rapidjson::Document doc;
        if (Utils::loadJson(path, &doc))
        {
            if (doc["layers"].IsArray())
            {
                entManager->clearData();
                auto assetManager = AssetManager::getInstance();
                assetManager->clearData();

                auto textures = doc["textures"].GetArray();
                for (size_t i = 0; i < textures.Size(); i++)
                {
                    auto tex = textures[i].GetJsonObject();
                    auto id = tex["id"].GetString();
                    auto path = tex["path"].GetString();
                    int columns = tex["columns"].GetInt();
                    int width = tex["width"].GetInt();
                    int height = tex["height"].GetInt();
                    assetManager->addTexture(id, path, columns, width, height);
                }

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
                auto nlayers = entManager->getLayers();
                if (layers.Size() > 0)
                {
                    auto second = nlayers->begin()->second;
                    entManager->setCurrentLayer(second.name);
                }
                success = true;
            }
            std::cout << "Time: " << SDL_GetTicks() - start << std::endl;
        }
        return success;
    }
} // namespace Plutus
