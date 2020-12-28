#include "Graphics/SpriteBatch2D.h"
#include "EntityManager.h"
#include "Graphics/GLSL.h"
#include <algorithm>
#include <cstdio>

#include "Utils.h"
#include "rapidjson/document.h"
#include "Serialize/Serialize.h"

#include "Components/Animate.h"
#include "Components/Sprite.h"
#include "Components/TileMap.h"
#include "Components/Transform.h"
#include "SceneLoader.h"

using namespace rapidjson;

namespace Plutus
{
	EntityManager::EntityManager()
	{
		mLayers["Layer0"].name = "Layer0";
		currentLayer = &mLayers["Layer0"];
	}

	void EntityManager::init()
	{
		mShader.CreateProgWithShader(vertexShader2, fragShader2);
		mShader.setAtribute("vertexPosition");
		mShader.setAtribute("vertexColor");
		mShader.setAtribute("vertexUV");
		mShader.enable();
		mSpriteBath2D.init();
	}

	void EntityManager::clearData()
	{
		for (auto &layer : mLayers)
		{
			layer.second.entities.clear();
		}
		mLayers.clear();

		for (auto &entity : mEntities)
		{
			entity->destroy();
			delete entity;
		}
		mEntities.clear();
	}

	bool EntityManager::hasNoEntities()
	{
		return mEntities.size() == 0;
	}

	void EntityManager::update(float dt)
	{

		for (int i = 0; i < mEntities.size(); i++)
		{
			if (mEntities[i]->isAlive())
			{
				mEntities[i]->update(dt);
			}
			else
			{
				delete mEntities[i];
				mEntities[i] = mEntities.back();
				mEntities.pop_back();
			}
		}
	}

	void EntityManager::draw()
	{
		mShader.enable();
		glActiveTexture(GL_TEXTURE0);
		mShader.setUniform1i("hasTexture", 0);
		mShader.setUniform1i("mySampler", 0);
		mShader.setUniformMat4("camera", mCamera->getCameraMatrix());
		for (auto &layer : mLayers)
		{
			size_t entCount = layer.second.entities.size();
			if (layer.second.isVisible && entCount > 0)
			{
				mSpriteBath2D.begin(entCount);
				for (auto &entity : layer.second.entities)
				{
					auto sprite = entity->getComponent<Sprite>();
					if (sprite)
					{

						auto trans = entity->getComponent<Transform>();
						sprite->mPosition = trans->position;
						sprite->mSize = trans->size;
						mSpriteBath2D.submit(sprite);
					}
				}
				mSpriteBath2D.end();
			}
		}
		mShader.disable();
	}

	Entity *EntityManager::addEntity(const std::string &entityName)
	{
		Entity *entity = new Entity(*this, entityName, currentLayer->name);
		mEntities.emplace_back(entity);
		currentLayer->entities.push_back(entity);
		return entity;
	}

	Entity *EntityManager::getEntity(const std::string &id)
	{
		for (auto &e : mEntities)
			if (e->name.compare(id) == 0)
				return e;

		return nullptr;
	}

	void EntityManager::setCamera(Camera2D *cm)
	{
		mCamera = cm;
	}

	std::vector<Entity *> EntityManager::getEntityList() const
	{
		return mEntities;
	}

	unsigned int EntityManager::getEntityCount()
	{
		return (int)mEntities.size();
	}

	Layer *EntityManager::addLayer(const std::string &layer)
	{
		mLayers[layer].isVisible = true;
		mLayers[layer].name = layer;
		currentLayer = &mLayers[layer];
		return currentLayer;
	}

	void EntityManager::addToLayer(Entity *e, const std::string &layer)
	{
		mLayers[layer].entities.emplace_back(e);
	}

	void EntityManager::setCurrentLayer(const std::string &layer)
	{
		currentLayer = &mLayers[layer];
	}

	std::unordered_map<std::string, Layer> *EntityManager::getLayers()
	{
		return &mLayers;
	}

	std::vector<Entity *> &EntityManager::getEntityByLayer(const std::string &layer)
	{
		return mLayers[layer].entities;
	}

	void EntityManager::removeEntity(Entity *e)
	{
		//Remove from Entities
		auto e1 = std::remove_if(mEntities.begin(), mEntities.end(), [e](Entity *entity) -> bool { return e == entity; });

		auto entities = &mLayers[e->mLayerId].entities;
		//Remove from Layer
		e1 = std::remove_if(entities->begin(), entities->end(), [e](Entity *entity) -> bool { return e == entity; });
		//Delete memory
		delete e;
	}

	void EntityManager::serialize(Serializer &ser)
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
					for (auto ent : layer.second.entities)
					{
						writer->StartObject();
						writer->String("name");
						writer->String(ent->name.c_str());
						writer->String("components");
						writer->StartArray();
						{
							for (auto comp : ent->getComponents())
							{
								comp->serialize(ser);
							}
						}
						writer->EndArray();
						writer->EndObject();
					}
				}
				writer->EndArray();
			}
			writer->EndObject();
		}
		writer->EndArray();

		writer->EndObject();
	}
} // namespace Plutus