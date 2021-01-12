#include "TileMap.h"
#include "Serialize/Serialize.h"
#include <algorithm>

namespace Plutus
{

    void TileMap::addTexture()
    {
    }

    void TileMap::removeTexture()
    {
    }

    void TileMap::serialize(Serializer &serializer)
    {
        auto writer = serializer.getWriter();
        writer->StartObject();
        {
            writer->String("name");
            writer->String("TileMap");
            writer->String("tilewidth");
            writer->Int(mTileWidth);
            writer->String("tileheight");
            writer->Int(mTileHeight);
            //Array of tileset name
            writer->String("tileset");
            writer->String(mTileset->name.c_str());

            //Array of textures
            writer->String("textures");
            writer->StartArray();
            for (auto tex : mTextures)
            {
                writer->String(tex->name.c_str());
            }
            writer->EndArray();
            //Tiles Array
            writer->String("tiles");
            writer->StartArray();
            {
                for (auto tile : mTiles)
                {
                    //Tile OBJ
                    writer->StartObject();
                    {
                        writer->String("t");
                        writer->Int(tile.texcoord);
                        writer->String("tx");
                        writer->Int(tile.texture);
                        writer->String("x");
                        writer->Int(tile.x);
                        writer->String("y");
                        writer->Int(tile.y);
                        writer->String("s");
                        writer->Bool(tile.isSolid);
                        writer->String("r");
                        writer->Double(tile.rotate);
                        writer->String("c");
                        writer->Int(tile.color);
                    }
                    writer->EndObject();
                }
            }
            writer->EndArray();
        }
        writer->EndObject();
    }

    int TileMap::tileIndex(const Tile &tile)
    {
        auto it = std::find_if(mTiles.begin(), mTiles.end(), [tile](const Tile &ntile) -> bool {
            return ntile.x == tile.x && ntile.y == tile.y; // && ntile.texId == tile.texId;
        });

        return it != mTiles.end() ? it - mTiles.begin() : -1;
    }
} // namespace Plutus
