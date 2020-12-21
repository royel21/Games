#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <vector>

namespace Plutus
{
	struct Animation
	{
		unsigned int startFrame;
		unsigned int numFrames;
		float aninSpeed;
		Animation() : startFrame(0), numFrames(1), aninSpeed(0.166f) {}

		Animation(unsigned int _startFrame, unsigned int _numFrames, int _aninSpeed = 100)
			: startFrame(_startFrame), numFrames(_numFrames), aninSpeed(1.0f / (_aninSpeed / 16.66f))
		{
		}
	};
} // namespace Plutus

#endif