#pragma once

#include <map>
#include <vector>
#include <string>
#include "Assets/AssetManager.h"
#include "Component.h"

namespace Plutus
{
    struct Animation
    {
        float aninSpeed;
        unsigned int startFrame;
        std::string textureId;
        std::vector<int> frames;
        Animation();
        Animation(unsigned int _startFrame, unsigned int _numFrames, int _aninSpeed = 100);
    };

    class Animate : public Component
    {
    public:
        bool loop = false;
        float aninTime = 0;
        TileSet *mTileset = nullptr;
        uint32_t currentFrame = 0;
        Animation *currentAnin = nullptr;
        std::map<std::string, Animation> animations;

    public:
        Animate() = default;
        Animate(float _aninTime, const std::string aninId, Animation anin);

        void update(float dt) override;
        void serialize(Serializer &serializer) override;

        void AddAnimation(const std::string id, const Animation &anin);
        void PlayAnimation(const std::string &aninName, bool _loop = false);
    };
} // namespace Plutus