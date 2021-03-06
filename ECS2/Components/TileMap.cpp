#include "TileMap.h"
#include "Assets/AssetsParser.h"
#include "Serialize/Serialize.h"

namespace Plutus
{
    TileMap::TileMap(const std::string &filePath)
    {
        AssetsParser::getInstance()->ParserTileMapFromJSON(filePath, this);
    }

    void TileMap::Serialize(Serializer &serializer)
    {
        auto writer = serializer.getWriter();
        writer->StartObject();
        writer->String("tilewidth");
        writer->Int(mTileWidth);
        writer->String("tileheight");
        writer->Int(mTileHeight);

        serializer.WriteMap("tilesets", mTilesets);

        writer->String("tiles");
        writer->StartArray();
        for (auto tile : mTiles)
        {
            writer->StartObject();
            writer->String("texId");
            writer->String(tile.texId.c_str());
            writer->String("coordId");
            writer->Int(tile.coordId);
            writer->String("x");
            writer->Int(tile.x);
            writer->String("y");
            writer->Int(tile.y);
            writer->EndObject();
        }
        writer->EndArray();

        writer->EndObject();
    }
} // namespace Plutus