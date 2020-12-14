#ifndef __ANIMATIONCOMPONENT_H__
#define __ANIMATIONCOMPONENT_H__

#include "Assets/AssetManager.h"

#include "Component.h"
#include "Animation.h"
#include "ImageComponent.h"

namespace Plutus
{
    class Serializer;

    class AnimationComponent : public Component
    {
        ImageComponent *mImage;
        TileSet *mTileset;

        bool animationLoop = false;
        int currentFrame = 0;
        float animationTime = 0;
        bool isAnimate = false;

        Animation *currentAni;
        std::map<std::string, Animation> animations;

    public:
        AnimationComponent(const std::string &tileSet);

        void Initialize() override;
        void Update(float deltaTime) override;

        void AddAnimation(const std::string id, const Animation &anin);
        void PlayAnimation(const std::string &animationName, bool loop = false);
        void setAnimationSpeed(int speed);
        void setFlipX(bool flip);
        void setFipY(bool flip);
        void Serialize(Serializer &writer) const override;
    };

} // namespace Plutus

#endif // __ANIMATIONCOMPONENT_H__