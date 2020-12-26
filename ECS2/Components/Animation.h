#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <vector>

namespace Plutus
{
	struct Animation
	{
		unsigned int startFrame;
		unsigned int numFrames;
		float animationSpeed;
		Animation() : startFrame(0), numFrames(1), animationSpeed(0.166f) {}

		Animation(unsigned int _startFrame, unsigned int _numFrames, int _animationSpeed = 100)
			: startFrame(_startFrame), numFrames(_numFrames), animationSpeed(1.0f / (_animationSpeed / 16.66f))
		{
		}
	};
} // namespace Plutus

#endif