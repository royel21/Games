#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H
#include <vector>
#include <bitset>
#include <glm/glm.hpp>
#include "Entity.h"
#include "Graphics/SpriteBatch2D.h"
#include "Graphics/Shader.h"
#include "Graphics/Camera2D.h"

namespace Plutus
{
	struct Tile
	{
		glm::vec2 position;
		unsigned short texcoord;
		unsigned short textureId;
	};

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
		SpriteBatch2D mSpriteBath2D;
		std::map<std::string, Layer> mLayers;
		Layer *currentLayer;
		Shader m_shader;
		Camera2D *m_camera = nullptr;

	public:
		static EntityManager *getInstance();

		void init();
		void Update(float dt);

		void Render();

		bool HasNoEntities();

		inline Layer *getCurrentLayer() { return currentLayer; }

		Entity &AddEntity(const std::string &entityName);

		Entity *GetEntity(const std::string &id);

		std::vector<Entity *> GetEntityList() const;

		unsigned int GetEntityCount();

		void setCamera(Camera2D *cm);

		void printEntity();

		void addLayer(const std::string &layer);

		void addToLayer(Entity *e, const std::string &layer);

		void setLayer(const std::string &layer);

		std::vector<Layer *> getLayers();

		std::vector<Entity *> &getEntityByLayer(const std::string &layer);

		void removeEntity(Entity *e);

		void setLayerVisivility(const std::string &layer, bool visibility);

		bool hasTile(int x, int y);

		void ClearData();

	private:
		EntityManager();
	};
} // namespace Plutus

#endif