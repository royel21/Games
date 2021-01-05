#include "Timing.h"
#include <SDL.h>
#include "Log/Logger.h"
namespace Plutus
{
	FpsLimiter::FpsLimiter() : mMaxFPS(60), mStartTick(0), mFps(60), mFrameTime(0)
	{
	}
	void FpsLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);
	}
	void FpsLimiter::setMaxFPS(float maxFPS)
	{
		mMaxFPS = 1000 / maxFPS;
	}
	double FpsLimiter::begin()
	{
		mStartTick = SDL_GetTicks();
		double dt = mStartTick - mLastTick;
		return dt;
	}
	float FpsLimiter::end()
	{
		float frameTicks = (float)(SDL_GetTicks() - mStartTick);
		mLastTick = mStartTick;
		// if (mMaxFPS > frameTicks)
		// {
		// 	while (mMaxFPS - (float)(SDL_GetTicks() - mStartTick) > 0)
		// 		;
		// }
		// Get samples for calculate fps
		if (mCurrentSample > FRAMESAMPLES)
		{
			mFps = 1000.0f / (mAcumulator / FRAMESAMPLES - 1);
			mAcumulator = 0;
			mCurrentSample = 0;
		}
		else
		{
			mAcumulator += (float)(SDL_GetTicks() - mStartTick);
			mCurrentSample++;
		}

		return frameTicks;
	}
} // namespace Plutus