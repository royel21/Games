#ifndef _TRANSFORM2DCOMPONENT_H
#define _TRANSFORM2DCOMPONENT_H

#include <glm/glm.hpp>
#include "Component.h"

namespace Plutus
{
	class Serializer;
	class Transform2DComponent : public Component
	{
	public:
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec2 size;
		float scale;
		Transform2DComponent(float x, float y, float velX, float velY, float w, float h, float _scale);

		void Initialize() override {}

		void Update(float dt) override
		{
			position += velocity * dt;
		}

		void Serialize(Serializer &serializer) const override;
	};
} // namespace Plutus
#endif
