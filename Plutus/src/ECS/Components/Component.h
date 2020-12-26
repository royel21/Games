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
		virtual void init(){};
		virtual void update(float deltaTime){};

		virtual void serialize(Serializer &writer){};
	};
} // namespace Plutus

#endif