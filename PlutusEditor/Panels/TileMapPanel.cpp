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
#include "Assets/AssetManager.h"

#define MODE_PLACE 0
#define MODE_EDIT 1
#define MODE_REMOVE 2

namespace Plutus
{
    void TileMapPanel::init(EntityManager *entManager)
    {
        mEntManager = entManager;
    }

    void TileMapPanel::addTexture(const char *label, bool &open, TileMap *tMap)
    {
        auto assestM = AssetManager::getInstance();
        if (open)
        {
            ImGui::SetNextWindowSize(ImVec2(300.0f, 270.0f));

            ImGui::OpenPopup(label);
            static char name[128];
            static std::string path = "", filename = "";
            static TileSet *tilesheet = nullptr;
            static int columns, tileWidth, tileHeight;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
            if (ImGui::BeginPopupModal(label, NULL, window_flags))
            {
                ImGui::PushItemWidth(100);
                ImGui::InputText("Name##modal", name, IM_ARRAYSIZE(name));
                ImGui::SameLine();
                static int selectedType = 0;
                const std::vector<std::string> type{"Texture", "Tilesheet"};
                ImGui::ComboBox("Type", type, selectedType);
                ImGui::PopItemWidth();
                ImGui::Separator();
                if (ImGui::Button("Load From File"))
                {
                    if (Utils::windowDialog(OPEN_FILE, path))
                    {
                        filename = Utils::getFileName(path);
                        std::cout << "Name: " << filename << "\n";
                        auto ex = Utils::getExtension(path);
                        if (ex.compare("png") != 0)
                        {
                            path = "";
                        }
                        else
                        {

                            if (selectedType == 1)
                            {
                                assestM->addTexture(name, path);
                            }
                            else
                            {
                                assestM->addTexture(name, path, columns, tileWidth, tileHeight);
                            }
                        }
                    }
                }
                ImGui::SameLine();
                ImGui::Text(filename.c_str());
                static bool fromList = false;
                ImGui::Checkbox("From List", &fromList);
                if (fromList)
                {
                    auto textures = assestM->getTilesets();

                    if (textures.size())
                    {
                        ImGui::Separator();
                        static auto selected = textures.begin()->first;
                        ImGui::ComboBox("Textures", textures, selected);
                        tilesheet = &textures[selected];
                        columns = tilesheet->columns;
                        tileWidth = tilesheet->tileWidth;
                        tileHeight = tilesheet->tileHeight;
                        ZeroMemory(name, 128);
                        strncpy(name, tilesheet->name.c_str(), tilesheet->name.size());
                    }
                }
                ImGui::Separator();
                if (selectedType == 1)
                {
                    ImGui::PushItemWidth(100);
                    ImGui::InputInt("Columns", &columns);
                    ImGui::InputInt("Tile Width", &tileWidth);
                    ImGui::InputInt("Tile Height", &tileHeight);
                    ImGui::PopItemWidth();
                    ImGui::Separator();
                }
                auto pos = ImGui::GetContentRegionAvail();
                ImGui::SetCursorPos(ImVec2(195.0f, 240.0f));
                if (ImGui::Button("save##modal"))
                {
                    std::string texName(name);
                    if (!texName.empty())
                    {
                        mTileMap->mTextures.push_back(assestM->getTexture(name));
                        open = false;
                        filename = "";
                        path = "";
                        selectedType = 0;
                        columns = 0;
                        tileWidth = 0;
                        tileHeight = 0;
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Close##modal"))
                    open = false;

                ImGui::EndPopup();
            }
        }
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
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
            if (ImGui::Button(ICON_FA_PLUS_CIRCLE " ##add-tilesheet"))
            {
                mShowAddModal = true;
            }
            ImGui::PopStyleColor();
            if (mShowAddModal)
            {
                addTexture("Add Texture To TileMap", mShowAddModal, mTileMap);
            }
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
            if (ImGui::Button(ICON_FA_TRASH_ALT " ##remove-tilesheet"))
            {
            }
            ImGui::PopStyleColor();
            ImGui::SameLine();
            if (mTileMap->mTextures.size() > 0)
            {
                ImGui::PushItemWidth(110);

                ImGui::ComboBox<TileSet>("TileSheet##mttexture", mTileMap->mTextures, mCurrentTexture);
                ImGui::PopItemWidth();
                ImGui::Separator();

                ImGui::RadioButton("Place", &mMode, MODE_PLACE);
                ImGui::SameLine();
                ImGui::RadioButton("Edit", &mMode, MODE_EDIT);
                ImGui::SameLine();
                ImGui::RadioButton("Remove", &mMode, MODE_REMOVE);
                ImGui::Separator();
                ImGui::TileSet(mTileMap->mTextures[mCurrentTexture], 1, mTempTiles);
            }
        }
    }

    void TileMapPanel::renderTiles(SpriteBatch2D *renderer, glm::ivec2 mCoords)
    {
        if (mTempTiles.size() && mMode == MODE_PLACE)
        {
            std::vector<Tile> tiles;
            for (auto tile : mTempTiles)
            {
                tiles.emplace_back(mCoords.x + tile.x, mCoords.y + tile.y, tile.z, mCurrentTexture, mRotation);
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
        int texcoord = mCurrentTile->texcoord;
        if (ImGui::InputInt("Texture##ctile", &texcoord, 1))
        {
            texcoord = LIMIT(texcoord, 0, mTileMap->mTileset->totalTiles - 1);
            mCurrentTile->texcoord = texcoord;
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
                else if (mTileMap->mTiles[index].texcoord != tile.texcoord)
                {
                    mTileMap->mTiles[index] = tile;
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
