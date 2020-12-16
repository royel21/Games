#ifndef _ENTITY_H
#define _ENTITY_H

#include <map>
#include <string>
#include <vector>
#include <typeinfo>
#include "Component.h"
#include "Serializer.h"

namespace Plutus
{
	class Serializer;
	class EntityManager;

	class Entity
	{
	private:
		bool isActive;
		EntityManager &manager;
		std::vector<Component *> components;
		std::map<const std::type_info *, Component *> componentTypeMap;

	public:
		std::string name;
		std::string mLayerId;
		Entity(EntityManager &manager, const std::string &layerId);
		Entity(EntityManager &manager, const std::string &name, const std::string &layerId);
		~Entity();

		void Update(float deltaTime);

		inline bool IsActive() const { return isActive; }

		template <typename T, typename... TArgs>
		T &addComponent(TArgs &&... args)
		{
			T *newComponent(new T(std::forward<TArgs>(args)...));
			newComponent->owner = this;

			components.emplace_back(newComponent);

			componentTypeMap[&typeid(*newComponent)] = newComponent;

			newComponent->Initialize();
			return *newComponent;
		}

		template <typename T>
		T *GetComponent()
		{
			return static_cast<T *>(componentTypeMap[&typeid(T)]);
		}

		void Destroy();

		void Serialize(Serializer &serializer) const
		{
			auto writer = serializer.getWriter();
			writer->StartObject();
			writer->String("Name");
			writer->String(name.c_str());

			writer->String("Components");
			writer->StartArray();

			for (auto &comp : components)
			{
				comp->Serialize(serializer);
			}

			writer->EndArray();

			writer->EndObject();
		}
	};
} // namespace Plutus
#endif