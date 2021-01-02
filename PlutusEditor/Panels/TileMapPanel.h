#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "Graphics/Shader.h"

namespace Plutus
{
    class TileMap;

    class EntityManager;
    class SpriteBatch2D;
    struct Tile;

    class TileMapPanel
    {
    private:
        EntityManager *mEntManager = nullptr;
        Shader mShader;
        int mMode = 0;
        //TileMap
        std::vector<glm::ivec3> mTempTiles;
        TileMap *mTileMap = nullptr;
        Tile *mCurrentTile = nullptr;
        float mRotation = 0;

    public:
        TileMapPanel() = default;
        void init(EntityManager *entManager);
        void draw(TileMap *tileMap);
        void drawTempTiles();
        void renderTiles(SpriteBatch2D *renderer, glm::ivec2 mCoords);
        void createTiles(const glm::ivec2 &mCoords);

    private:
        static bool compare(const glm::ivec2 &a, const glm::ivec2 &b);
        void tileProps();
    };
} // namespace Plutus
