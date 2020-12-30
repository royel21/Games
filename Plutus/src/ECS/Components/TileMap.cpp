#include "TileMap.h"
#include "Serialize/Serialize.h"

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
                        writer->Bool(tile.isSolid);
                    }
                    writer->EndObject();
                }
            }
            writer->EndArray();
        }
        writer->EndObject();
    }
} // namespace Plutus
