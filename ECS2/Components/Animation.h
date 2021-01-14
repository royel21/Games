#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <vector>
#include <string>

namespace Plutus
{
	struct Animation
	{
		float aninSpeed;
		unsigned int startFrame;
		std::string textureId;
		std::vector<int> frames;
		Animation() : startFrame(0), aninSpeed(0.166f) {}

		Animation(unsigned int _startFrame, unsigned int _numFrames, int _aninSpeed = 100)
			: startFrame(_startFrame), aninSpeed(1.0f / (_aninSpeed / 16.66f))
		{
		}
	};
} // namespace Plutus

#endif