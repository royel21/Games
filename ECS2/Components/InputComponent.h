#ifndef __INPUTCOMPONENT_H__
#define __INPUTCOMPONENT_H__

#include "Component.h"
#include "Input/GamePad.h"
#include "Input/InputManager.h"
#include "Transform2DComponent.h"
#include "AnimationComponent.h"
#include "ImageComponent.h"

namespace Plutus
{
    class InputComponent : public Component
    {
    private:
        InputManager *m_input;
        GamePad *m_gamepad;
        Transform2DComponent *m_transfComp = nullptr;
        AnimationComponent *m_aninComp = nullptr;
        ImageComponent *m_imgComp = nullptr;

    public:
        InputComponent(){};

        void Initialize();
        void Update(float deltaTime);
        void Serialize(Serializer &writer) const;
    };
} // namespace Plutus

#endif // __INPUTCOMPONENT_H__