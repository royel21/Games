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
                // LOG_I("{0:x} {1} {2} {3}", state.Gamepad.sThumbLX, state.Gamepad.sThumbLY, state.Gamepad.sThumbRX, state.Gamepad.sThumbRY);
                gamepadList[i] = btnMap;
            }
        }
    }
} // namespace Plutus

/*
    GP_UP       0001
    GP_DOWN     0002
    GP_LEFT     0004
    GP_RIGHT    0008
    GP_Start    0010
    GP_Select   0020
    GP_L3       0040
    GP_R3       0080
    GP_L1       0100
    GP_R1       0200
    GP_B       1000
    GP_A       2000
    GP_Y       4000
    GP_X       8000
    GP_R2       XXXX
    GP_L2       XXXX
*/