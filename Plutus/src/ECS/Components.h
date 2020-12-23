#pragma once
#include <iostream>
#include "glm/glm.hpp"
#include "Assets/AssetManager.h"
#include "Animation.h"

namespace Plutus
{
    class Renderable2D;

    struct TagComponent
    {
        TagComponent() = default;
        TagComponent(const std::string &_tag) : Tag(_tag) {}
        std::string Tag;
    };

    struct TransformComponent
    {
        glm::vec2 position;
        glm::vec2 size;
        int rotation;
        TransformComponent() = default;

        TransformComponent(float x, float y, int w, int h, int r = 0)
        {
            position = glm::vec2(x, y);
            size = glm::vec2(w, h);
            rotation = r;
        }

        TransformComponent(const glm::vec2 &pos, glm::vec2 _size, int r = 0)
        {
            position = pos;
            size = size;
            rotation = r;
        }
    };

    struct AnimationComponent
    {

        bool loop = false;
        float aninTime = 0;
        TileSet *mTileset = nullptr;
        uint32_t currentFrame = 0;
        Animation *currentAnin = nullptr;
        std::map<std::string, Animation> animations;

        AnimationComponent() = default;

        AnimationComponent(float _aninTime, const std::string aninId, Animation anin) : aninTime(_aninTime)
        {
            animations[aninId] = anin;
            currentAnin = &anin;
        }

        void AddAnimation(const std::string id, const Animation &anin)
        {
            animations[id] = anin;
        }

        void PlayAnimation(const std::string &aninName, bool _loop = false)
        {
            currentAnin = &animations[aninName];
            loop = _loop;
        }

        // void setAnimationSpeed(int speed) { currentAnin->aninSpeed = speed; }
    };

} // namespace Plutus