#pragma once

#include "glm/glm.hpp"
#include "Graphics/vertex.h"
#include "Component.h"
#include <string>

namespace Plutus
{
    class Sprite : public Component
    {
    public:
        glm::vec2 mPosition;
        glm::vec2 mSize;
        glm::vec4 mUVCoord;
        ColorRGBA8 mColor;
        std::string mTextureId;
        bool mFlipX = false;
        bool mFlipY = false;
        float mRotation = 0.0f;
        GLuint mTexId;

    public:
        Sprite();
        Sprite(std::string textId);

        void update(float dt) override;
        void serialize(Serializer &serializer) override;
    };
} // namespace Plutus