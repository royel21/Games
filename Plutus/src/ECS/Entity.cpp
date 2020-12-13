#include "ECS/Entity.h"
#include "ECS/Component.h"
#include "ECS/EntityManager.h"

namespace Plutus
{
	Entity::Entity(EntityManager &_manager, const std::string &layerId) : manager(_manager), mLayerId(layerId)
	{
		isActive = true;
	}

	Entity::Entity(EntityManager &_manager, const std::string &_name, const std::string &layerId) : manager(_manager), name(_name), mLayerId(layerId)
	{
		isActive = true;
	}

	Entity::~Entity()
	{
		for (auto &c : components)
		{
			delete c;
		}
		components.clear();
	}

	void Entity::Update(float deltaTime)
	{
		for (auto &c : components)
		{
			c->Update(deltaTime);
		}
	}

	void Entity::Destroy()
	{
		isActive = false;
	}
} // namespace Plutus