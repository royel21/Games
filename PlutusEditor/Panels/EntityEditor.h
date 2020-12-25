#pragma once
#include <vector>
#include "imgui.h"

#define EDIT_PLACE 0
#define EDIT_SELECT 1
#define EDIT_REMOVE 2

namespace Plutus
{
    class Scene;
    class EntityEditor
    {
    public:
        EntityEditor() = default;
        void init(Scene *scene);
        void draw();

    private:
        int mMode;
        std::vector<ImVec2> mSelectedtiles;
        Scene *mScene;
    };
} // namespace Plutus