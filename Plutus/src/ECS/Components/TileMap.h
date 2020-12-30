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
        Tile(int _x, int _y, int _texId, bool _isSolid)
        {
            x = _x;
            y = _y;
            texId = _texId;
            isSolid = _isSolid;
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
    };
} // namespace Plutus