#pragma once

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
	class SceneLoader;

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
		friend SceneLoader;

	public:
		EntityManager();

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

		Layer *addLayer(const std::string &layer);

		void addToLayer(Entity *e, const std::string &layer);

		void setCurrentLayer(const std::string &layer);

		std::unordered_map<std::string, Layer> *getLayers();

		std::vector<Entity *> &getEntityByLayer(const std::string &layer);

		void removeEntity(Entity *e);

		void clearData();

		void serialize(Serializer &serializer);

		void startDraw();
		void endDraw() { mShader.disable(); }
		SpriteBatch2D *getRenderer() { return &mSpriteBath2D; }
	};
} // namespace Plutus