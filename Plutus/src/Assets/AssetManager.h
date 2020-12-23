#ifndef _ASSETMANAGER_H
#define _ASSETMANAGER_H

#include "Texture/TextureManager.h"

#include <map>
#include <string>
#include <vector>
#include "glm/glm.hpp"

namespace Plutus
{

	struct TileSet
	{
		std::string name;
		int column;
		int width;
		int height;
		GLTexture texture;
		std::vector<glm::vec4> coords;
		TileSet(const std::string &_name, int _column, int _width, int _height, GLTexture tex) : name(_name), column(_column), width(_width), height(_height), texture(tex)
		{
			int size = column * int(texture.height / _height);
			for (int i = 0; i < size; i++)
			{
				int y = i / column;
				int x = i % column;
				glm::vec4 UV;
				UV.x = ((float)(x * width) / (float)texture.width);
				UV.y = ((float)(y * height) / (float)texture.height);
				UV.z = ((float)(x * width + width) / (float)texture.width);
				UV.w = ((float)(y * height + height) / (float)texture.height);

				coords.push_back(UV);
			}
		}

		const glm::vec4 getUV(int texcoord) { return coords[texcoord]; }
	};

	class AssetManager
	{
	public:
		~AssetManager();

		static void clearData();

		static GLTexture addTexture(const std::string &textureId, const std::string &texturePath);

		static void addTileSet(const std::string &id, int column, int width, int height, const std::string &texturePath);

		static TileSet *getTileSet(const std::string &id);

		static GLTexture getTexture(const std::string &textureId);

		static void setTextureFilter(GLuint filter, bool min) { TextureManager::setTextureFilter(filter, min); }

		static std::vector<TileSet *> &getTilesets() { return tilesets; };

	private:
		static std::map<std::string, std::string> textures;
		static std::vector<TileSet *> tilesets;
	};
} // namespace Plutus

#endif