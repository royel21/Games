#pragma once

#include <cstring>
#include <vector>
#include <unordered_map>
#include "Assets/AssetManager.h"
#include "Component.h"

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

    class TileMap : public Component
    {
    public:
        int mTileWidth;
        int mTileHeight;
        std::vector<Tile> mTiles;
        std::unordered_map<std::string, TileSet *> mTilesets;

    public:
        TileMap() = default;
        TileMap(int w, int h) : mTileWidth(w), mTileHeight(h) {}
        void serialize(Serializer &serializer) override;
    };
} // namespace Plutus