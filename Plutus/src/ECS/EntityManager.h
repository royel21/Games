#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H
#include <vector>
#include <bitset>
#include <unordered_map>
#include "glm/glm.hpp"

#include "Entity.h"
#include "Graphics/SpriteBatch2D.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera2D.h"

namespace Plutus
{
	class Serializer;

	struct Layer
	{
		std::string name;
		bool isVisible = true;
		std::vector<Entity *> entities;
	};

	class EntityManager
	{
	private:
		std::vector<Entity *> mEntities;
		std::unordered_map<std::string, Layer> mLayers;
		Layer *currentLayer;
		Shader mShader;
		Camera2D *mCamera = nullptr;
		SpriteBatch2D mSpriteBath2D;

	public:
		static EntityManager *getInstance();

		void init();
		void update(float dt);

		void draw();

		bool hasNoEntities();

		inline Layer *getCurrentLayer() { return currentLayer; }

		Entity *addEntity(const std::string &entityName);

		Entity *getEntity(const std::string &id);

		std::vector<Entity *> getEntityList() const;

		unsigned int getEntityCount();

		void setCamera(Camera2D *cm);

		void addLayer(const std::string &layer);

		void addToLayer(Entity *e, const std::string &layer);

		void setCurrentLayer(const std::string &layer);

		std::unordered_map<std::string, Layer> *getLayers();

		std::vector<Entity *> &getEntityByLayer(const std::string &layer);

		void removeEntity(Entity *e);

		void clearData();

		void serialize(Serializer &serializer);

		void deserialize(const std::string jsonFile);

	private:
		EntityManager();
	};
} // namespace Plutus

#endif