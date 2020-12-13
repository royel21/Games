
#include "GamePad.h"
#include "Win32/XinputHandler.h"

namespace Plutus
{
    GamePad *GamePad::gamepad;

    GamePad::GamePad()
    {
    }

    GamePad *GamePad::getInstance()
    {
        if (!gamepad)
            gamepad = new GamePad();

        return gamepad;
    }
    unsigned long GamePad::getButtons(int index)
    {
        return m_buttons[index].BUTTONS;
    }

    bool GamePad::isConnected(int index = 0)
    {
        return m_buttons[index].isConnected;
    }

    void GamePad::Update()
    {
        for (auto &key : m_buttons)
        {
            m_prevbuttons[key.first] = key.second;
        }
        updateGamePadState(m_buttons);
    }

    bool GamePad::wasBtnDown(int index, long buttons)
    {
        auto it = m_prevbuttons.find(index);
        //Check if key was found and return it state else return false
        return it != m_prevbuttons.end() ? it->second.BUTTONS & buttons : false;
    }

    bool GamePad::isBtnDown(int index, long button)
    {
        return m_buttons[index].BUTTONS & button;
    }

    bool GamePad::isBtnPress(int index, long button)
    {
        return m_buttons[index].BUTTONS & button && !wasBtnDown(index, button);
    }
} // namespace Plutus
