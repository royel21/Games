#ifndef _ASSETMANAGER_H
#define _ASSETMANAGER_H

#include "Texture/TextureManager.h"

#include <unordered_map>
#include <string>
#include <vector>
#include "glm/glm.hpp"

namespace Plutus
{
	class Serializer;

	struct TileSet
	{
		std::string name;
		int columns;
		int tileWidth;
		int tileHeight;
		int texWidth;
		int texHeight;
		int totalTiles;
		uint32_t texId;
		std::string path;
		std::vector<glm::vec4> uvs;
		int mTexCount = 0;
		TileSet() : columns(0), tileWidth(0), tileHeight(0), texWidth(0), texHeight(0), texId(0) {}

		TileSet(const std::string &id, const std::string &p, int c, int w, int h, GLTexture tex) : name(id), path(p)
		{
			texId = tex.id;
			texWidth = tex.width;
			texHeight = tex.height;
			tileWidth = w;
			tileHeight = h;
			columns = c;
			totalTiles = 0;
			if (h)
			{
				totalTiles = static_cast<int>(tex.width / h) * static_cast<int>(tex.height / h);
			}
			calculateUV();
		}

		void calculateUV()
		{
			if (tileWidth > 0)
			{
				mTexCount = columns * int(texHeight / tileHeight);
				for (int i = 0; i < mTexCount; i++)
				{
					int y = i / columns;
					int x = i % columns;
					glm::vec4 UV;
					UV.x = ((float)(x * tileWidth) / (float)texWidth);
					UV.y = ((float)(y * tileHeight) / (float)texHeight);
					UV.z = ((float)(x * tileWidth + tileWidth) / (float)texWidth);
					UV.w = ((float)(y * tileHeight + tileHeight) / (float)texHeight);
					uvs.push_back(UV);
				}
			}
		}
		inline const glm::vec4 getUV(int texcoord)
		{
			return texcoord < mTexCount ? uvs[texcoord] : glm::vec4();
		}
	};
	//Manage all the texture of the game
	class AssetManager
	{
	public:
		static AssetManager *getInstance();
		~AssetManager();

		const TileSet &addTexture(const std::string &id, const std::string &path);
		const TileSet &addTexture(const std::string &id, const std::string &path, int c, int w, int h);

		TileSet *getTexture(const std::string &id);
		std::unordered_map<std::string, TileSet> &getTilesets() { return tilesets; };

		void setTextureFilter(GLuint filter, bool min) { TextureManager::setTextureFilter(filter, min); }
		void Serialize(Serializer &serializer);
		void clearData();

	private:
		AssetManager();

		std::unordered_map<std::string, TileSet> tilesets;
	};
} // namespace Plutus

#endif