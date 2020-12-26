#ifndef _ENTITY_H
#define _ENTITY_H

#include <unordered_map>
#include <string>
#include <vector>
#include <typeinfo>
#include "Components/Component.h"

namespace Plutus
{
	class EntityManager;

	class Entity
	{
	private:
		bool isActive;
		EntityManager &manager;
		std::vector<Component *> components;
		std::unordered_map<const std::type_info *, Component *> componentTypeMap;

	public:
		std::string name;
		std::string mLayerId;
		Entity(EntityManager &manager, const std::string &layerId);
		Entity(EntityManager &manager, const std::string &name, const std::string &layerId);
		~Entity();

		void update(float deltaTime);

		inline std::vector<Component *> getComponents() { return components; };

		inline bool isAlive() const { return isActive; }

		template <typename T, typename... TArgs>
		T &addComponent(TArgs &&... args)
		{
			T *newComponent(new T(std::forward<TArgs>(args)...));
			newComponent->owner = this;

			components.emplace_back(newComponent);

			componentTypeMap[&typeid(*newComponent)] = newComponent;

			newComponent->init();
			return *newComponent;
		}

		template <typename T>
		T *getComponent()
		{
			return static_cast<T *>(componentTypeMap[&typeid(T)]);
		}

		template <typename T>
		bool hasComponent()
		{
			return componentTypeMap[&typeid(T)];
		}

		void destroy();
	};
} // namespace Plutus
#endif