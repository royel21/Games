#include "AnimationComponent.h"
#include "ECS/Serializer.h"
#include "Log/Logger.h"

namespace Plutus
{
    AnimationComponent::AnimationComponent(const std::string &tileSet)
    {
        mImage = nullptr;
        currentAni = nullptr;
        mTileset = AssetManager::getTileSet(tileSet);
    }

    void AnimationComponent::Initialize()
    {
        mImage = owner->GetComponent<ImageComponent>();
    }

    void AnimationComponent::Update(float deltaTime)
    {
        if (isAnimate)
        {
            animationTime += currentAni->animationSpeed * deltaTime;

            currentFrame = currentAni->startFrame + (int)animationTime % currentAni->numFrames;

            mImage->setUV(mTileset->getUV(currentFrame));
        }
        else
        {
            mImage->setUV(mTileset->getUV(currentAni->startFrame));
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