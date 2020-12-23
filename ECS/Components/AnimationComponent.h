#ifndef __ANIMATIONCOMPONENT_H__
#define __ANIMATIONCOMPONENT_H__

#include "Assets/AssetManager.h"

#include "Component.h"
#include "Animation.h"

namespace Plutus
{
    class Serializer;

    class AnimationComponent : public Component
    {
        TileSet *mTileset;

        bool animationLoop = false;
        int currentFrame = 0;
        float animationTime = 0;
        bool isAnimate = false;

        Animation *currentAni;
        std::map<std::string, Animation> animations;

    public:
        AnimationComponent(const std::string &tileSet);
        void Update(float deltaTime) override;

        void AddAnimation(const std::string id, const Animation &anin);
        void PlayAnimation(const std::string &animationName, bool loop = false);
        inline void setAnimate(bool state) { isAnimate = state; }
        void setAnimationSpeed(int speed);
        void Serialize(Serializer &writer) const override;
    };

} // namespace Plutus

#endif // __ANIMATIONCOMPONENT_H__