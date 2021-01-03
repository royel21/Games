#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "imgui.h"
#include "glm/glm.hpp"

#define EDIT_PLACE 0
#define EDIT_SELECT 1
#define EDIT_REMOVE 2

namespace Plutus
{
    class InputManager;
    class EntityManager;
    class Entity;
    class EditorUI;
    struct Layer;

    class EntityEditor
    {
    public:
        EntityEditor() = default;
        void init(EntityManager *emanager, EditorUI *parent);
        void draw();
        void loadScene(std::string path);

    private:
        void layers();
        void entity();
        void drawTileset();
        std::string LayerModal(char *label, bool *open);

    private:
        int mMode;
        glm::vec2 modalPos;
        std::vector<ImVec2> mSelectedtiles;
        bool mShowCreateLayer = false;
        InputManager *mInputManager = nullptr;

        EntityManager *mEntManager = nullptr;
        Layer *mCurrentLayer = nullptr;
        std::unordered_map<std::string, Layer> *mLayers;
        std::string mCurLayerName;
        EditorUI *mParentUI;
    };
} // namespace Plutus