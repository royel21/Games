#ifndef __GAMEPADBUTTONS_H__
#define __GAMEPADBUTTONS_H__

#ifdef WIN32

#define GAMEPAD_UP 0x0001
#define GAMEPAD_DOWN 0x0002
#define GAMEPAD_LEFT 0x0004
#define GAMEPAD_RIGHT 0x0008

#define GAMEPAD_START 0x0010 // Start
#define GAMEPAD_BACK 0x0020  // Select

#define GAMEPAD_L3 0x0040 // Left
#define GAMEPAD_R3 0x0080 // Right

#define GAMEPAD_L2 0x0100 // L2
#define GAMEPAD_R2 0x0200 // R2

#define GAMEPAD_A 0x1000 // B
#define GAMEPAD_B 0x2000 // A
#define GAMEPAD_X 0x4000 // Y
#define GAMEPAD_Y 0x8000 // X
#else
#define GAMEPAD_UP 0x0001
#define GAMEPAD_DOWN 0x0002
#define GAMEPAD_LEFT 0x0004
#define GAMEPAD_RIGHT 0x0008

#define GAMEPAD_START 0x0010 // Start
#define GAMEPAD_BACK 0x0020  // Select

#define GAMEPAD_L3 0x0040 // Left
#define GAMEPAD_R3 0x0080 // Right

#define GAMEPAD_L2 0x0100 // L2
#define GAMEPAD_R2 0x0200 // R2

#define GAMEPAD_A 0x1000 // B
#define GAMEPAD_B 0x2000 // A
#define GAMEPAD_X 0x4000 // Y
#define GAMEPAD_Y 0x8000 // X
#endif

namespace Plutus
{
    struct ButtonMap
    {
        bool isConnected = false;
        long long BUTTONS = 0;
        bool L2 = 0;
        bool R2 = 0;
        short LStickX = 0;
        short LStickY = 0;
        short RStickX = 0;
        short RStickY = 0;
    };
} // namespace Plutus

#endif // __GAMEPADBUTTONS_H__