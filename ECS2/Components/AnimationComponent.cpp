#include "AnimationComponent.h"
#include "ECS/Serializer.h"
#include "Log/Logger.h"
#include "ImageComponent.h"

namespace Plutus
{
    AnimationComponent::AnimationComponent(const std::string &tileSet)
    {
        currentAni = nullptr;
        mTileset = AssetManager::getTileSet(tileSet);
    }

    void AnimationComponent::Update(float deltaTime)
    {
        auto imgConmp = owner->GetComponent<ImageComponent>();
        if (isAnimate)
        {
            animationTime += currentAni->animationSpeed * deltaTime;

            currentFrame = currentAni->startFrame + (int)animationTime % currentAni->numFrames;

            imgConmp->setUV(mTileset->getUV(currentFrame));
        }
        else
        {
            imgConmp->setUV(mTileset->getUV(currentAni->startFrame));
        }
    }

    void AnimationComponent::AddAnimation(const std::string id, const Animation &anin)
    {
        animations[id] = anin;
    }

    void AnimationComponent::PlayAnimation(const std::string &animationName, bool loop)
    {
        currentAni = &animations[animationName];
        animationLoop = loop;
        isAnimate = loop;
    }

    void AnimationComponent::setAnimationSpeed(int speed)
    {
        currentAni->animationSpeed = speed ? 1.0f / (speed / 16.66f) : 0.166f;
    }

    void AnimationComponent::Serialize(Serializer &writer) const
    {
    }
} // namespace Plutus