#include "Timer.h"
#include "SDL.h"

namespace Plutus
{
    T_Timer::T_Timer() : deltaTime(0.0f), lastTime(0.0f)
    {
    }

    void T_Timer::Tick()
    {
        deltaTime = SDL_GetTicks() - lastTime;
        lastTime = SDL_GetTicks();
    }

    void T_Timer::Initialize()
    {
    }
} // namespace Plutus