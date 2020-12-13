#include "ECS/Transform2DComponent.h"
#include "ECS/Serializer.h"

namespace Plutus
{
	Transform2DComponent::Transform2DComponent(float x, float y, float velX, float velY, float w, float h, float _scale)
	{
		position = glm::vec2(x, y);
		velocity = glm::vec2(velX, velY);
		size = glm::vec2(w, h);
		scale = _scale;
	}

	void Transform2DComponent::Serialize(Serializer &serializer) const
	{
		auto writer = serializer.getWriter();
		//auto writer = (PrettyWriter<StringBuffer>*)wr;
		writer->StartObject();
		//Type
		writer->String("Type");
		writer->String("Transform");
		// Position
		writer->String("Position");
		writer->StartObject();
		{
			writer->String("x");
			writer->Double(position.x);
			writer->String("y");
			writer->Double(position.y);
		}
		writer->EndObject();
		//Velocity
		writer->String("Velocity");
		writer->StartObject();
		{
			writer->String("x");
			writer->Double(velocity.x);
			writer->String("y");
			writer->Double(velocity.y);
		}
		writer->EndObject();
		//Size
		writer->String("Size");
		writer->StartObject();
		{
			writer->String("x");
			writer->Double(size.x);
			writer->String("y");
			writer->Double(size.y);
		}
		writer->EndObject();
		//Scale
		writer->String("Scale");
		writer->Double(scale);

		writer->EndObject();
	}
} // namespace Plutus