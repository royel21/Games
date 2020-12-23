#ifndef __IMAGECOMPONENT_H__
#define __IMAGECOMPONENT_H__

#include "ECS/Entity.h"
#include "Graphics/Renderable2D.h"
#include "Component.h"
#include "Transform2DComponent.h"

namespace Plutus
{
    class Renderable2D;

    class ImageComponent : public Component, public Renderable2D
    {
    protected:
        std::string mTextureId;
        Transform2DComponent *transform;

    public:
        ImageComponent(const std::string &textureId);
        void Update(float deltaTime) override;
        void Serialize(Serializer &serializer) const override;
    };

} // namespace Plutus
#endif // __IMAGECOMPONENT_H__