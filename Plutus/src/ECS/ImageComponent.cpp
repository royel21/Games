#include "ImageComponent.h"
#include "Assets/AssetManager.h"

Plutus::ImageComponent::ImageComponent(const std::string &textureId)
{
    mTextureId = textureId;
    auto &texture = AssetManager::getTexture(textureId);
    mTid = texture.id;
}

void Plutus::ImageComponent::Initialize()
{
    transform = owner->GetComponent<Transform2DComponent>();
}

void Plutus::ImageComponent::Update(float deltaTime)
{
    mPosition = transform->position;
    mSize = transform->size;
}

void Plutus::ImageComponent::Serialize(Serializer &serializer) const
{
}
