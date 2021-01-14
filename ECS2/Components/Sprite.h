#pragma once

#include "glm/glm.hpp"
#include "Graphics/vertex.h"

namespace Plutus
{
    class Serializer;

    class Sprite
    {
    public:
        glm::vec2 mPosition;
        glm::vec2 mSize;
        glm::vec4 mUVCoord;
        ColorRGBA8 mColor;
        GLuint mTextureId;
        bool mFlipX = false;
        bool mFlipY = false;
        float mRotation = 0.0f;
        Sprite();
        Sprite(GLuint textId);
        void Serialize(Serializer &serializer);
    };
} // namespace Plutus