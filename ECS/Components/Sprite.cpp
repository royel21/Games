#include "Sprite.h"
#include "Serialize/Serialize.h"

namespace Plutus
{
    Sprite::Sprite() : mPosition(0), mSize(0), mUVCoord(0, 1, 1, 0), mTextureId(0)
    {
    }

    Sprite::Sprite(GLuint textId) : mPosition(0), mSize(0), mUVCoord(0, 1, 1, 0), mTextureId(textId)
    {
    }

    void Sprite::Serialize(Serializer &serializer)
    {
        auto writer = serializer.getWriter();
        writer->String("Sprite");
        writer->StartObject();
        {
            writer->String("texture");
            writer->String("mTextureId");
        }
        writer->EndObject();
    }
} // namespace Plutus