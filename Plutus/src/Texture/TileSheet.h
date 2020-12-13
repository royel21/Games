#ifndef _TILESHEET_H
#define _TILESHEET_H

#include "glm/glm.hpp"
#include "TextureManager.h"

namespace Plutus
{
	struct TileSheet
	{
		GLTexture texture;
		glm::ivec2 dims;

		glm::vec4 getUVs(int index)
		{
			int xTile = index % dims.x;
			int yTile = index / dims.x;

			glm::vec4 uvs;

			uvs.x = xTile / (float)dims.x;
			uvs.z = (xTile + 1.0f) / dims.x;

			uvs.y = yTile / (float)dims.y;
			uvs.w = (yTile + 1.0f) / dims.y;

			return uvs;
		}
	};
} // namespace Plutus
#endif // TileSheet.h