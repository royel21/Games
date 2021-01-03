#pragma once
#include <vector>
#include "imgui.h"
#include "TileMapPanel.h"

namespace Plutus
{
    class Entity;
    struct TileSet;
    class AssetManager;
    class InputManager;
    class EntityManager;
    class EditorUI;
    class SpriteBatch2D;

    class ComponentPanel
    {
    private:
        Entity *mEntity = nullptr;
        AssetManager *mAManager = nullptr;
        InputManager *mInputManager = nullptr;
        std::vector<ImVec2> mSelectedtiles;
        EntityManager *mEntManager = nullptr;
        TileMapPanel mTileMapPanel;
        glm::vec2 mGridCoord;

    public:
        ComponentPanel() = default;

        void init(EntityManager *entmanager);
        void drawUI(Entity *ent);
        void render(SpriteBatch2D *renderer, glm::vec2 mcoords);
        inline void createTiles(const glm::vec2 &mCoords) { mTileMapPanel.createTiles(mCoords); }

    private:
        void drawAnimate();
        void drawTransform();
        void drawSprite();
        void drawTileMap();
        void drawCanvas(TileSet *tileset, float scale);
        void drawTexCoords(TileSet *tileset, float scale);
        void drawTilesetProps(TileSet *tileset);
        bool showCreateComp(bool &open);
        std::vector<std::string> getCompList();
    };
} // namespace Plutus