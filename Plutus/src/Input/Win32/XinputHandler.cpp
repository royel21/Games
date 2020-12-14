#include <windows.h>
#include <Xinput.h>
#include <unordered_map>

#include "XinputHandler.h"
#include "Log/Logger.h"
#include "Input/GamePadButtons.h"

#pragma comment(lib, "Xinput.lib")

namespace Plutus
{
    short StickValue(short value)
    {
        return value;
    }

    void updateGamePadState(std::unordered_map<unsigned int, ButtonMap> &gamepadList)
    {
        DWORD dwResult;

        for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
        {
            XINPUT_STATE state;
            ButtonMap btnMap;
            ZeroMemory(&state, sizeof(XINPUT_STATE));

            // Simply get the state of the controller from XInput.
            dwResult = XInputGetState(i, &state);
            if (dwResult == 0)
            {
                btnMap.isConnected = dwResult == 0;
                btnMap.BUTTONS = state.Gamepad.wButtons;
                btnMap.L2 = state.Gamepad.bLeftTrigger;
                btnMap.R2 = state.Gamepad.bRightTrigger;
                btnMap.LStickX = state.Gamepad.sThumbLX;
                btnMap.LStickY = state.Gamepad.sThumbLY;
                btnMap.RStickX = state.Gamepad.sThumbRX;
                btnMap.RStickY = state.Gamepad.sThumbRY;
                gamepadList[i] = btnMap;
            }
        }
    }
} 