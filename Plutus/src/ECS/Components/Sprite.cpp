#include "Sprite.h"
#include "Serialize/Serialize.h"
#include "Assets/AssetManager.h"

namespace Plutus
{
    Sprite::Sprite() : mPosition(0), mSize(0), mUVCoord(0, 0, 1, 1), mTextureId(0)
    {
    }

    Sprite::Sprite(std::string textId) : mPosition(0), mSize(0), mUVCoord(0, 0, 1, 1), mTextureId(textId)
    {
        mTexId = AssetManager::getInstance()->getTexture(textId)->texId;
    }

    void Sprite::update(float dt)
    {
    }

    void Sprite::serialize(Serializer &serializer)
    {
        auto writer = serializer.getWriter();
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Sprite");
            writer->String("texture");
            writer->String(mTextureId.c_str());
            writer->String("color");
            writer->Int(mColor.get());
        }
        writer->EndObject();
    }
} // namespace Plutus