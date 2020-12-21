#pragma once
#include <iostream>
#include "glm/glm.hpp"
#include "Assets/AssetManager.h"
#include "Animation.h"

namespace Plutus
{

    struct TagComponent
    {
        TagComponent() = default;
        TagComponent(const std::string tag) : Tag(tag) {}
        std::string Tag;
    };

    struct TransformComponent
    {
        glm::vec3 position;
        int rotation;
        TransformComponent() = default;
        TransformComponent(float x, float y, float z = 1, int _rotation = 0) : position(x, y, z), rotation(_rotation) {}
        TransformComponent(const glm::vec3 &pos, int _rotation = 0) : position(pos), rotation(_rotation) {}
        TransformComponent(const glm::vec2 pos, int _rotation = 0) : position(pos.x, pos.y, 0), rotation(_rotation) {}
    };

    struct TextureComponent
    {
        uint32_t textureId;
        glm::vec4 uvCoords;
        TextureComponent() = default;
        TextureComponent(uint32_t textId, const glm::vec4 _uvCoords) : textureId(textId), uvCoords(_uvCoords) {}
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
        void setAnimationSpeed(int speed) { currentAnin->aninSpeed = speed; }
    };

} // namespace Plutus