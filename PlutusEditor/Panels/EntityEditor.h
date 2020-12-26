#pragma once
#include <vector>
#include "imgui.h"
#include "glm/glm.hpp"
#include "ComponentPanel.h"

#define EDIT_PLACE 0
#define EDIT_SELECT 1
#define EDIT_REMOVE 2

namespace Plutus
{
    class EntityManager;

    class EntityEditor
    {
    public:
        EntityEditor() = default;
        void init(EntityManager *emanager);
        void draw();
        void drawEntity();
        void LayerModal();

    private:
        int mMode;
        EntityManager *mEntManager;
        glm::vec2 modalPos;
        std::vector<ImVec2> mSelectedtiles;
        bool mShowCreateLayer = false;
        ComponentPanel mComPanel;
    };
} // namespace Plutus