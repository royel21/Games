#pragma once
#include "glm/glm.hpp"
#include "Component.h"

namespace Plutus
{
    class Transform : public Component
    {
    public:
        glm::vec2 position;
        glm::vec2 size;
        int rotation;

    public:
        Transform() = default;
        Transform(float x, float y, int w, int h, int r = 0);
        Transform(const glm::vec2 &pos, glm::vec2 _size, int r = 0);
        void update(float dt) override;
        void serialize(Serializer &serializer) override;
    };
} // namespace Plutus