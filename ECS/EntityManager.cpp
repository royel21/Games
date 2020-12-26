#include "Graphics/SpriteBatch2D.h"
#include "ECS/Components/ImageComponent.h"
#include "EntityManager.h"
#include "Graphics/GLSL.h"

#include <algorithm>

#include <cstdio>

using namespace rapidjson;

namespace Plutus
{
	EntityManager::EntityManager()
	{
		mLayers["Layer0"].name = "Layer0";
		currentLayer = &mLayers["Layer0"];
	}

	EntityManager *EntityManager::getInstance()
	{
		static EntityManager manager = EntityManager();
		return &manager;
	}

	void EntityManager::init()
	{

		m_shader.CreateProgWithShader(vertexShader2, fragShader2);
		m_shader.setAtribute("vertexPosition");
		m_shader.setAtribute("vertexColor");
		m_shader.setAtribute("vertexUV");
		m_shader.enable();
		mSpriteBath2D.init();
	}

	void EntityManager::ClearData()
	{
		for (auto &layer : mLayers)
		{
			layer.second.entities.clear();
		}
		mLayers.clear();

		for (auto &entity : mEntities)
		{
			entity->Destroy();
			delete entity;
		}
		mEntities.clear();
	}

	bool EntityManager::HasNoEntities()
	{
		return mEntities.size() == 0;
	}

	void EntityManager::Update(float dt)
	{

		for (int i = 0; i < mEntities.size(); i++)
		{
			if (mEntities[i]->IsActive())
			{
				mEntities[i]->Update(dt);
			}
			else
			{
				delete mEntities[i];
				mEntities[i] = mEntities.back();
				mEntities.pop_back();
			}
		}
	}

	void EntityManager::Render()
	{
		m_shader.enable();
		glActiveTexture(GL_TEXTURE0);
		m_shader.setUniform1i("hasTexture", 0);
		m_shader.setUniform1i("mySampler", 0);
		m_shader.setUniformMat4("camera", m_camera->getCameraMatrix());
		for (auto &layer : mLayers)
		{
			if (layer.second.isVisible && layer.second.entities.size() > 0)
			{
				mSpriteBath2D.begin();
				for (auto &entity : layer.second.entities)
				{
					Renderable2D *r2d = entity->GetComponent<ImageComponent>();
					if (r2d)
					{
						mSpriteBath2D.submit(r2d);
					}
				}
				mSpriteBath2D.end();
				mSpriteBath2D.flush();
			}
		}
		m_shader.disable();
	}

	Entity &EntityManager::AddEntity(const std::string &entityName)
	{
		Entity *entity = new Entity(*this, entityName, currentLayer->name);
		mEntities.emplace_back(entity);
		currentLayer->entities.push_back(entity);
		return *entity;
	}

	Entity *EntityManager::GetEntity(const std::string &id)
	{
		for (auto &e : mEntities)
			if (e->name.compare(id) == 0)
				return e;

		return nullptr;
	}

	void EntityManager::setCamera(Camera2D *cm)
	{
		m_camera = cm;
	}

	std::vector<Entity *> EntityManager::GetEntityList() const
	{
		return mEntities;
	}

	unsigned int EntityManager::GetEntityCount()
	{
		return (int)mEntities.size();
	}
	void EntityManager::printEntity()
	{
		Serializer ser;
		ser.writer.StartArray();

		for (auto &e : mEntities)
		{
			e->Serialize(ser);
		}
		ser.writer.EndArray();
		puts(ser.sb.GetString());
	}

	void EntityManager::addLayer(const std::string &layer)
	{
		mLayers[layer].isVisible = true;
		mLayers[layer].name = layer;
		currentLayer = &mLayers[layer];
	}

	void EntityManager::addToLayer(Entity *e, const std::string &layer)
	{
		mLayers[layer].entities.emplace_back(e);
	}

	void EntityManager::setLayer(const std::string &layer)
	{
		currentLayer = &mLayers[layer];
	}

	std::vector<Layer *> EntityManager::getLayers()
	{
		std::vector<Layer *> layers;
		for (auto &layer : mLayers)
		{
			layers.push_back(&layer.second);
		}
		return layers;
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

	void EntityManager::setLayerVisivility(const std::string &layer, bool visibility)
	{
		mLayers[layer].isVisible = visibility;
	}

	bool EntityManager::hasTile(int x, int y)
	{
		auto entities = currentLayer->entities;

		auto it = std::find_if(entities.begin(), entities.end(),
							   [x, y](Entity *entity) -> bool {
								   auto c = entity->GetComponent<Transform2DComponent>();
								   return c->position.x == x && c->position.y == y;
							   });

		return it != entities.end();
	}
} // namespace Plutus