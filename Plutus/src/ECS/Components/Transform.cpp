
#include "Serialize/Serialize.h"
#include "Transform.h"

namespace Plutus
{
    Transform::Transform(float x, float y, int w, int h, int r)
    {
        position = glm::vec2(x, y);
        size = glm::vec2(w, h);
        rotation = r;
    }

    Transform::Transform(const glm::vec2 &pos, glm::vec2 _size, int r)
    {
        position = pos;
        size = size;
        rotation = r;
    }

    void Transform::update(float dt)
    {
    }

    void Transform::serialize(Serializer &serializer)
    {
        auto writer = serializer.getWriter();
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Transform");
            writer->String("x");
            writer->Int(position.x);
            writer->String("y");
            writer->Int(position.y);
            writer->String("w");
            writer->Int(size.x);
            writer->String("h");
            writer->Int(size.y);
        }
        writer->EndObject();
    }
} // namespace Plutus