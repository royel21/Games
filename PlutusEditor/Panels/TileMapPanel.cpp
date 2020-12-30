#include "TileMapPanel.h"
#include "ECS/Components/TileMap.h"
#include "imgui.h"
#include "ImGuiEx.h"

namespace Plutus
{
    void TileMapPanel::draw(TileMap *mTileMap)
    {
        if (ImGui::CollapsingHeader("TileMap##comp"))
        {
            int size[] = {mTileMap->mTileWidth, mTileMap->mTileHeight};
            ImGui::PushItemWidth(60);
            if (ImGui::DragInt2("Tile Size", size, 1))
            {
                mTileMap->mTileWidth = size[0];
                mTileMap->mTileHeight = size[1];
            }
            ImGui::PopItemWidth();
            auto assets = AssetManager::getInstance();
            static std::vector<glm::ivec2> selected;
            if (ImGui::TileSet(mTileMap->mTileset, 1, selected))
            {
            }
        }
    }
} // namespace Plutus
