#pragma once

namespace Plutus
{
    class TileMap;

    class TileMapPanel
    {
    public:
        TileMapPanel() = default;
        void draw(TileMap *mTileMap);
    };
} // namespace Plutus
