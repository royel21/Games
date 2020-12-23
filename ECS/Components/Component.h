#ifndef _COMPONENT_H
#define _COMPONENT_H

namespace Plutus
{
	class Entity;
	class Serializer;

	class Component
	{
	public:
		Entity *owner;
		virtual ~Component() {}
		virtual void Initialize(){};
		virtual void Update(float deltaTime) = 0;

		virtual void Serialize(Serializer &writer) const = 0;
	};
} // namespace Plutus

#endif