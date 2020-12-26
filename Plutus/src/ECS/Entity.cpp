#include "Entity.h"
#include "Components/Component.h"
#include "EntityManager.h"

namespace Plutus
{
	Entity::Entity(EntityManager &_manager, const std::string &layerId)
		: manager(_manager), mLayerId(layerId)
	{
		isActive = true;
	}

	Entity::Entity(EntityManager &_manager, const std::string &_name, const std::string &layerId)
		: manager(_manager), name(_name), mLayerId(layerId)
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

	void Entity::update(float deltaTime)
	{
		for (auto &c : components)
		{
			c->update(deltaTime);
		}
	}

	void Entity::destroy()
	{
		isActive = false;
	}
} // namespace Plutus