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

		void begin();
		//return the fps
		float end();

	private:
		float m_maxFPS;
		unsigned int m_startTick;

		float m_fps = 0;
		float m_frameTime;
		float acumulator;
		float frameCounter = 0;
		const int FRAMESAMPLES = 30;
		int currentSample = 0;

		///New Timer
	};

}

#endif