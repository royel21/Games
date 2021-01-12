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
        int texcoord;
        int texture = 0;
        bool isSolid = false;
        float rotate = 0.0f;
        unsigned int color = 0xffffffff;

        Tile(int _x, int _y, int _texcoord, unsigned int _texture = 0, float r = 0.0f, bool _isSolid = false, unsigned _color = 0xffffffff)
        {
            x = static_cast<float>(_x);
            y = static_cast<float>(_y);
            texcoord = _texcoord;
            texture = _texture;
            isSolid = _isSolid;
            rotate = r;
            color = color;
        }

        inline bool Tile::operator==(Tile tile)
        {
            return (x == tile.x && y == tile.y && texcoord == tile.texcoord);
        }

        inline bool Tile::operator!=(Tile tile)
        {
            return (x != tile.x && y != tile.y && texcoord != tile.texcoord);
        }
    };

    class TileMap : public Component
    {
    public:
        int mTileWidth;
        int mTileHeight;
        std::vector<Tile> mTiles;
        TileSet *mTileset = nullptr;
        std::vector<TileSet *> mTextures;

    public:
        TileMap() = default;
        TileMap(int tileWidth, int tileHeight) : mTileWidth(tileWidth), mTileHeight(tileHeight) {}
        void serialize(Serializer &serializer) override;
        int tileIndex(const Tile &tile);
        void addTexture();
        void removeTexture();
    };
} // namespace Plutus