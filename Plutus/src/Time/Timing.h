#ifndef _TIMING_H
#define _TIMING_H

typedef unsigned long long PU64;

namespace Plutus
{
	class FpsLimiter
	{
	public:
		FpsLimiter();
		void init(float maxFPS);

		void setMaxFPS(float maxFPS);
		float getFPS() { return mFps; };
		double begin();
		//return the fps
		float end();

	private:
		float mMaxFPS;
		unsigned int mStartTick;

		float mFps = 0;
		float mFrameTime;
		float mAcumulator;
		float mFrameCounter = 0;
		const int FRAMESAMPLES = 100;
		int mCurrentSample = 0;
		unsigned int mLastTick = 0;

		///New Timer
	};

} // namespace Plutus

#endif