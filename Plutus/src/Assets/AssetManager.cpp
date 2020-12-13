#include "AssetManager.h"
#include <algorithm>

namespace Plutus
{
	std::map<std::string, std::string> AssetManager::textures;
	std::vector<TileSet *> AssetManager::tilesets;

	AssetManager::~AssetManager()
	{
	}

	void AssetManager::clearData()
	{
		TextureManager::destroyTextures();
		for (auto &ts : tilesets)
		{
			delete ts;
		}
		tilesets.clear();
	}

	void AssetManager::addTexture(const std::string &textureId, const std::string &texturePath)
	{
		textures.emplace(textureId, texturePath);
	}

	void AssetManager::addTileSet(const std::string &name, int column, int width, int height, const std::string &texturePath)
	{
		addTexture(name, texturePath);
		tilesets.push_back(new TileSet(name, column, width, height, getTexture(name)));
	}

	TileSet *AssetManager::getTileSet(const std::string &id)
	{
		for (auto ts : tilesets)
		{
			if (id.compare(ts->name) == 0)
				return ts;
		}
		return nullptr;
	}

	GLTexture AssetManager::getTexture(const std::string &textureId)
	{
		return TextureManager::getTexture(textures[textureId]);
	}

} // namespace Plutus
