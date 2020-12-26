#include "ImageComponent.h"
#include "Assets/AssetManager.h"
namespace Plutus
{

    ImageComponent::ImageComponent(const std::string &textureId)
    {
        mTextureId = textureId;
        auto &texture = AssetManager::getTexture(textureId);
        mTid = texture.id;
    }

    void ImageComponent::Update(float deltaTime)
    {
        auto transform = owner->GetComponent<Transform2DComponent>();
        mPosition = transform->position;
        mSize = transform->size;
    }

    void ImageComponent::Serialize(Serializer &serializer) const
    {
    }

    void ImageComponent::setFlipX(bool f) { flipX = f; }
    void ImageComponent::setFlipY(bool f) { flipY = f; }
} // namespace Plutus