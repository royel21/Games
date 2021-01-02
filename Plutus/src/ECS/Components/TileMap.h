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
        float x;
        float y;
        int texId;
        bool isSolid = false;
        float rotate = 0.0f;

        Tile(int _x, int _y, int _texId, float r = 0.0f, bool _isSolid = false)
        {
            x = static_cast<float>(_x);
            y = static_cast<float>(_y);
            texId = _texId;
            isSolid = _isSolid;
            rotate = r;
        }

        inline bool Tile::operator==(Tile tile)
        {
            return (x == tile.x && y == tile.y && texId == tile.texId);
        }

        inline bool Tile::operator!=(Tile tile)
        {
            return (x != tile.x && y != tile.y && texId != tile.texId);
        }
    };

    class TileMap : public Component
    {
    public:
        int mTileWidth;
        int mTileHeight;
        std::vector<Tile> mTiles;
        TileSet *mTileset = nullptr;

    public:
        TileMap() = default;
        TileMap(int tileWidth, int tileHeight) : mTileWidth(tileWidth), mTileHeight(tileHeight) {}
        void serialize(Serializer &serializer) override;
        int tileIndex(const Tile &tile);
    };
} // namespace Plutus