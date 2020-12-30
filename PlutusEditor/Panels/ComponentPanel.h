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

    class ComponentPanel
    {
    private:
        Entity *mEntity = nullptr;
        AssetManager *mAManager = nullptr;
        InputManager *mInputManager = nullptr;
        std::vector<ImVec2> mSelectedtiles;
        EntityManager *mEntManager = nullptr;
        TileMapPanel mTileMapPanel;

    public:
        ComponentPanel() = default;

        void init(EntityManager *entmanager);
        void drawUI(Entity *ent);
        void drawAnimate();
        void drawTransform();
        void drawSprite();
        void drawTileMap();
        void drawCanvas(TileSet *tileset, float scale);
        void drawTexCoords(TileSet *tileset, float scale);

        void drawTilesetProps(TileSet *tileset);
        void setEntity(Entity *ent) { mEntity = ent; };
    };
} // namespace Plutus