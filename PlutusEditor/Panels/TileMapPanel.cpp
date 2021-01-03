#include "TileMapPanel.h"
#include "ECS/Components/TileMap.h"
#include "imgui.h"
#include "ImGuiEx.h"
#include "Log/Logger.h"
#include <iostream>
#include <algorithm>
#include "ECS/EntityManager.h"
#include "Graphics/SpriteBatch2D.h"
#include "Log/Logger.h"
#include "Utils.h"

#define MODE_PLACE 0
#define MODE_EDIT 1
#define MODE_REMOVE 2

namespace Plutus
{
    void TileMapPanel::init(EntityManager *entManager)
    {
        mEntManager = entManager;
    }

    void TileMapPanel::draw(TileMap *tileMap)
    {
        mTileMap = tileMap;
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
            ImGui::PushItemWidth(85);
            if (mCurrentTile != nullptr && mMode == MODE_EDIT)
            {
                tileProps();
            }
            else
            {
                if (ImGui::InputFloat("Rotation", &mRotation, 45.0f, 90.0f, "%0.0f"))
                {
                    mRotation = LIMIT(mRotation, 0.0f, 360.0f);
                }
            }

            ImGui::PopItemWidth();
            auto assets = AssetManager::getInstance();
            ImGui::Separator();

            ImGui::RadioButton("Place", &mMode, MODE_PLACE);
            ImGui::SameLine();
            ImGui::RadioButton("Edit", &mMode, MODE_EDIT);
            ImGui::SameLine();
            ImGui::RadioButton("Remove", &mMode, MODE_REMOVE);
            ImGui::Separator();
            ImGui::TileSet(mTileMap->mTileset, 1, mTempTiles);
        }
    }

    void TileMapPanel::renderTiles(SpriteBatch2D *renderer, glm::ivec2 mCoords)
    {
        if (mTempTiles.size() && mMode == MODE_PLACE)
        {
            std::vector<Tile> tiles;
            for (auto tile : mTempTiles)
            {
                tiles.emplace_back(mCoords.x + tile.x, mCoords.y + tile.y, tile.z, mRotation);
            }
            renderer->begin(1);
            renderer->submit(mTileMap->mTileWidth, mTileMap->mTileHeight, tiles, mTileMap->mTileset);
            renderer->end();
        }
    }

    void TileMapPanel::tileProps()
    {
        ImGui::Separator();
        ImGui::Text("Tile Props");
        ImGui::Separator();
        int x = mCurrentTile->x;
        int y = mCurrentTile->y;
        if (ImGui::InputInt("X##ctile", &x, 1))
        {
            mCurrentTile->x = x;
        }
        ImGui::SameLine();
        if (ImGui::InputInt("Y##ctile", &y, 1))
        {
            mCurrentTile->y = y;
        }

        float rotation = mCurrentTile->rotate;
        if (ImGui::InputFloat("Rotation##ctile", &rotation, 45.0f, 90.0f, "%0.0f"))
        {
            rotation = LIMIT(rotation, 0.0f, 360.0f);
            mCurrentTile->rotate = rotation;
        }
        int texId = mCurrentTile->texId;
        if (ImGui::InputInt("Texture##ctile", &texId, 1))
        {
            texId = LIMIT(texId, 0, mTileMap->mTileset->totalTiles - 1);
            mCurrentTile->texId = texId;
        }
    }

    void TileMapPanel::createTiles(const glm::ivec2 &mCoords)
    {
        auto tiles = &mTileMap->mTiles;
        switch (mMode)
        {
        case MODE_PLACE:
        {
            for (auto tile : mTempTiles)
            {
                Tile tile(mCoords.x + tile.x, mCoords.y + tile.y, tile.z, mRotation);
                int index = mTileMap->tileIndex(tile);
                if (index == -1)
                {
                    tiles->push_back(tile);
                }
                else if (mTileMap->mTiles[index].texId != tile.texId)
                {
                    tiles->insert(tiles->begin(), index, tile);
                }
            }
            break;
        }
        case MODE_EDIT:
        {
            Tile tile(mCoords.x, mCoords.y, 0);
            int index = mTileMap->tileIndex(tile);
            if (index > -1)
            {
                mCurrentTile = &mTileMap->mTiles[index];
            }
            else
            {
                mCurrentTile = nullptr;
            }
            break;
        }
        default:
        {
            auto found = std::remove_if(tiles->begin(), tiles->end(), [mCoords](const Tile &tile) -> bool {
                return static_cast<int>(tile.x) == mCoords.x && static_cast<int>(tile.y) == mCoords.y;
            });
            tiles->erase(found, tiles->end());
        }
        }
    }

    bool TileMapPanel::compare(const glm::ivec2 &a, const glm::ivec2 &b)
    {
        if (a.x < b.x)
            return true;
        if (a.x > b.x)
            return false;
        if (a.y < b.y)
            return true;
        if (a.y > b.y)
            return false;
        return false;
    }
} // namespace Plutus
