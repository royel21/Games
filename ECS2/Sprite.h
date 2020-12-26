#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <glm/glm.hpp>
#include "Graphics/vertex.h"

namespace Plutus
{
    class Sprite
    {
    public:
        glm::vec2 mPosition;
        glm::vec2 mSize;
        glm::vec4 mUVCoord;
        ColorRGBA8 mColor;
        GLuint mTid;
        bool flipX = false;
        bool flipY = false;
        Sprite() : mPosition(0), mSize(0), mUVCoord(0, 0, 1, 1), mTid(0)
        {
        }
        Sprite(
            const glm::vec2 &position,
            const glm::vec2 &size,
            const glm::vec4 &uvCoord,
            const ColorRGBA8 &color,
            uint32_t tid,
            float angle = 0) : mPosition(position), mSize(size), mUVCoord(uvCoord), mColor(color), mTid(tid)
        {
        }
    };
} // namespace Plutus

#endif // __SPRITE_H__