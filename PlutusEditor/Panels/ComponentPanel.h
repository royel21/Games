#pragma once
#include <vector>
#include "imgui.h"

namespace Plutus
{
    class Entity;
    struct TileSet;
    class AssetManager;
    class InputManager;

    class ComponentPanel
    {
    public:
        Entity *mEntity = nullptr;
        AssetManager *mAManager = nullptr;
        InputManager *mInputManager = nullptr;
        std::vector<ImVec2> mSelectedtiles;

    public:
        ComponentPanel() = default;

        void init();
        void drawUI();
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