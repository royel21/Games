#pragma once
#include "Graphics/Renderable2D.h"

namespace Plutus
{
    struct SpriteComponent
    {
        glm::vec2 mPosition;
        glm::vec2 mSize;
        glm::vec4 mUVCoord;
        ColorRGBA8 mColor;
        GLuint mTextureId;
        bool mFlipX = false;
        bool mFlipY = false;
        float mRotation = 0.0f;
        SpriteComponent() : mPosition(0), mSize(0), mUVCoord(0, 0, 1, 1), mTextureId(0)
        {
        }
        SpriteComponent(GLuint textId) : mPosition(0), mSize(0), mUVCoord(0, 0, 1, 1), mTextureId(textId)
        {
        }
    };
} // namespace Plutus