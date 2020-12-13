#include "Sprite2D.h"
namespace Plutus
{
	Sprite2D::Sprite2D(
		const glm::vec2& pos,
		const glm::vec2& size,
		const glm::vec4& uvCoord,
		const ColorRGBA8& color,
		GLuint textId,
		float angle
	) : Renderable2D(pos, size, uvCoord, color, textId, angle)
	{
	}
}