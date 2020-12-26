#include "Transform2DComponent.h"
#include "ECS/Serializer.h"
#include "Log/Logger.h"

namespace Plutus
{
	Transform2DComponent::Transform2DComponent(float x, float y, float vX, float vY, float w, float h, float _scale)
		: position(x, y), velocity(vX, vY), size(w, h), scale(_scale)
	{
	}

	Transform2DComponent::Transform2DComponent(glm::vec2 pos, float vX, float vY, float w, float h, float _scale)
		: position(pos), velocity(vX, vY), size(w, h), scale(_scale)
	{
	}

	Transform2DComponent::Transform2DComponent(glm::vec2 pos, float w, float h, float _scale)
		: position(pos), velocity(0, 0), size(w, h), scale(_scale)
	{
	}

	Transform2DComponent::Transform2DComponent(float x, float y, float w, float h, float _scale)
		: position(x, y), velocity(0, 0), size(w, h), scale(_scale)
	{
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