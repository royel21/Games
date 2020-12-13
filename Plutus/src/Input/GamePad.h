#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include "GamePadButtons.h"
#include <unordered_map>

namespace Plutus
{
    class GamePad
    {
    public:
        static GamePad *getInstance();
        bool isConnected(int index);
        void Update();
        bool isBtnPress(int index, long button);
        bool isBtnDown(int index, long button);
        unsigned long GamePad::getButtons(int index);
        inline unsigned char getL2(int index) { return m_buttons[index].L2; }
        inline unsigned char getR2(int index) { return m_buttons[index].R2; }
        inline unsigned char getLstickX(int index) { return m_buttons[index].LStickX; }
        inline unsigned char getLstickY(int index) { return m_buttons[index].LStickY; }
        inline unsigned char getRstickX(int index) { return m_buttons[index].RStickX; }
        inline unsigned char getRstickY(int index) { return m_buttons[index].RStickY; }

    private:
        GamePad();
        bool wasBtnDown(int index, long buttons);
        static GamePad *gamepad;
        std::unordered_map<unsigned int, ButtonMap> m_buttons;
        std::unordered_map<unsigned int, ButtonMap> m_prevbuttons;
    };

} // namespace Plutus

#endif // __GAMEPAD_H__