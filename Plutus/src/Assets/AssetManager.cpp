#include "AssetManager.h"
#include <algorithm>
#include <sstream>
#include "Serialize/Serialize.h"

namespace Plutus
{
	AssetManager::AssetManager()
	{
	}
	AssetManager *AssetManager::getInstance()
	{
		static AssetManager instance;
		return &instance;
	}

	AssetManager::~AssetManager()
	{
		TextureManager::destroyTextures();
	}

	void AssetManager::clearData()
	{
		tilesets.clear();
		TextureManager::destroyTextures();
	}

	const TileSet &AssetManager::addTexture(const std::string &id, const std::string &path)
	{
		return addTexture(id, path, 0, 0, 0);
	}

	const TileSet &AssetManager::addTexture(const std::string &id, const std::string &path, int c, int w, int h)
	{

		GLTexture tex = TextureManager::getTexture(path);
		tilesets[id] = TileSet(id, path, c, w, h, tex);
		return tilesets[id];
	}

	TileSet *AssetManager::getTexture(const std::string &id)
	{
		return &tilesets[id];
	}

	void AssetManager::Serialize(Serializer &serializer)
	{
		auto writer = serializer.getWriter();
		writer->String("textures");
		writer->StartArray();
		for (auto tile : tilesets)
		{
			writer->StartObject();

			writer->String("id");
			writer->String(tile.first.c_str());
			writer->String("path");
			writer->String(tile.second.path.c_str());
			writer->String("columns");
			writer->Int(tile.second.columns);
			writer->String("width");
			writer->Int(tile.second.tileWidth);
			writer->String("height");
			writer->Int(tile.second.tileHeight);

			writer->EndObject();
		}
		writer->EndArray();
	}

} // namespace Plutus
