#include "Timing.h"
#include <SDL.h>

namespace Plutus
{
	FpsLimiter::FpsLimiter() : m_maxFPS(60), m_startTick(0), m_fps(60), m_frameTime(0)
	{
	}
	void FpsLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);
	}
	void FpsLimiter::setMaxFPS(float maxFPS)
	{
		m_maxFPS = 1000 / maxFPS;
	}
	void FpsLimiter::begin()
	{
		m_startTick = SDL_GetTicks();
	}
	float FpsLimiter::end()
	{
		float frameTicks = (float)(SDL_GetTicks() - m_startTick);
		if (m_maxFPS > frameTicks)
		{
			while (m_maxFPS - (float)(SDL_GetTicks() - m_startTick) > 0);
		}
		// Get samples for calculate fps
		if (currentSample > FRAMESAMPLES)
		{
			m_fps = 1000.0f / (acumulator / FRAMESAMPLES-1);
			acumulator = 0;
			currentSample = 0;
		} else
		{
			acumulator += (float)(SDL_GetTicks() - m_startTick);
			currentSample++;
		}

		return m_fps;
	}
}