#include "InputComponent.h"
#include "SDL.h"
#include "Input/GamePadButtons.h"
#include "Log/Logger.h"

namespace Plutus
{
    void InputComponent::Initialize()
    {
        m_input = InputManager::getInstance();
        m_gamepad = GamePad::getInstance();
        m_transfComp = owner->GetComponent<Transform2DComponent>();
        m_aninComp = owner->GetComponent<AnimationComponent>();
        m_imgComp = owner->GetComponent<ImageComponent>();
    }

    void InputComponent::Update(float deltaTime)
    {
        m_aninComp->setAnimate(false);
        if (m_input->onKeyDown(SDLK_UP) || m_gamepad->isBtnDown(0, GAMEPAD_UP) || (m_gamepad->isConnected(0) && m_gamepad->getLstickY(0) > 150))
        {
            m_transfComp->position.y += 5;
            m_aninComp->PlayAnimation("walk-up", true);
        }
        if (m_input->onKeyDown(SDLK_DOWN) || m_gamepad->isBtnDown(0, GAMEPAD_DOWN) || (m_gamepad->isConnected(0) && m_gamepad->getLstickY(0) < 110))
        {
            m_transfComp->position.y -= 5;
            m_aninComp->PlayAnimation("walk-down", true);
        }
        if (m_input->onKeyDown(SDLK_RIGHT) || m_gamepad->isBtnDown(0, GAMEPAD_RIGHT) || (m_gamepad->isConnected(0) && m_gamepad->getLstickX(0) > 150))
        {
            m_transfComp->position.x += 5;
            m_imgComp->setFlipX(false);
            m_aninComp->PlayAnimation("run", true);
        }
        if (m_input->onKeyDown(SDLK_LEFT) || m_gamepad->isBtnDown(0, GAMEPAD_LEFT) || (m_gamepad->isConnected(0) && m_gamepad->getLstickX(0) < 110))
        {
            m_transfComp->position.x -= 5;
            m_imgComp->setFlipX(true);
            m_aninComp->PlayAnimation("run", true);
        }
    }

    void InputComponent::Serialize(Serializer &writer) const
    {
    }
} // namespace Plutus