#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "imgui.h"
#include "glm/glm.hpp"
#include "ComponentPanel.h"

#define EDIT_PLACE 0
#define EDIT_SELECT 1
#define EDIT_REMOVE 2

namespace Plutus
{
    class InputManager;
    class EntityManager;
    class Entity;
    struct Layer;

    class EntityEditor
    {
    public:
        EntityEditor() = default;
        void init(EntityManager *emanager);
        void draw();
        void loadScene();

    private:
        void drawEntity();
        void layers();
        void entity();
        void drawTileset();
        std::string LayerModal(char *label, bool *open);

    private:
        int mMode;
        glm::vec2 modalPos;
        std::vector<ImVec2> mSelectedtiles;
        bool mShowCreateLayer = false;
        ComponentPanel mComPanel;
        InputManager *mInputManager = nullptr;

        EntityManager *mEntManager = nullptr;
        Layer *mCurrentLayer = nullptr;
        Entity *mCurrentEnt = nullptr;
        std::unordered_map<std::string, Layer> *mLayers;
        std::string mCurLayerName;
    };
} // namespace Plutus