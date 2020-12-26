#pragma once

#include <cstring>
#include <vector>
#include <unordered_map>
#include "Assets/AssetManager.h"

namespace Plutus
{
    class Serializer;

    struct Tile
    {
        std::string texId;
        float x;
        float y;
        int coordId;
        Tile(const std::string _textId, int _x, int _y, int _coord)
        {
            x = _x;
            y = _y;
            texId = _textId;
            coordId = _coord;
        }
    };

    class TileMap
    {
    public:
        int mTileWidth;
        int mTileHeight;
        std::vector<Tile> mTiles;
        std::unordered_map<std::string, TileSet *> mTilesets;

        TileMap() = default;
        TileMap(const std::string &filePath);
        void Serialize(Serializer &serializer);
    };
} // namespace Plutus