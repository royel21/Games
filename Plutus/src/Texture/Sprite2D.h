#ifndef _SPRITE2D_H
#define _SPRITE2D_H

#include "Graphics/Renderable2d.h"
#include "Graphics/vertex.h"

namespace Plutus
{
	class Sprite2D : public Renderable2D
	{
	public:
		Sprite2D(
			const glm::vec2 &pos,
			const glm::vec2 &size,
			const glm::vec4 &uvCoord,
			const ColorRGBA8 &color,
			GLuint textId = 0,
			float angle = 0);
	};

} // namespace Plutus
#endif