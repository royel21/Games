#include "TileMap.h"
#include "Serialize/Serialize.h"
#include <algorithm>

namespace Plutus
{
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
            //Tiles Array
            writer->String("tiles");
            writer->StartArray();
            {
                for (auto tile : mTiles)
                {
                    //Tile OBJ
                    writer->StartObject();
                    {
                        writer->String("texId");
                        writer->Int(tile.texId);
                        writer->String("x");
                        writer->Int(tile.x);
                        writer->String("y");
                        writer->Int(tile.y);
                        writer->String("isSolid");
                        writer->Bool(tile.isSolid);
                        writer->String("rotate");
                        writer->Double(tile.rotate);
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
