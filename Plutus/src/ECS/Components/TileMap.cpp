#include "TileMap.h"
#include "Assets/AssetsParser.h"
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
            serializer.WriteMap("tilesets", mTilesets);
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
                        writer->String(tile.texId.c_str());
                        writer->String("coordId");
                        writer->Int(tile.coordId);
                        writer->String("x");
                        writer->Int(tile.x);
                        writer->String("y");
                        writer->Int(tile.y);
                    }
                    writer->EndObject();
                }
            }
            writer->EndArray();
        }
        writer->EndObject();
    }
} // namespace Plutus
