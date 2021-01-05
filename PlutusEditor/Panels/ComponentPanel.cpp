#include "ComponentPanel.h"
#include "glm/glm.hpp"
#include "SDL.h"

#include "Assets/AssetManager.h"
#include "Input/InputManager.h"
#include "ImGuiEx.h"

#include "ECS/Components/Animate.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/TileMap.h"
#include "ECS/EntityManager.h"
#include "ECS/Entity.h"
#include "Graphics/SpriteBatch2D.h"
#include "Utils.h"

#define COMP_TRASNFORM 0
#define COMP_SPRITE 1
#define COMP_ANIMATE 2

#define CHECKLIMIT(val, min, max) val<min ? min : val> max ? max : val

const uint32_t color1 = IM_COL32(50, 50, 50, 255);
const uint32_t color2 = IM_COL32(50, 50, 60, 255);
const uint32_t color3 = IM_COL32(60, 60, 70, 255);

namespace Plutus
{
    void ComponentPanel::init(EntityManager *entManager)
    {
        mEntManager = entManager;
        mAManager = AssetManager::getInstance();
        mInputManager = InputManager::getInstance();
        mTileMapPanel.init(entManager);
    }
    void ComponentPanel::drawUI(Entity *ent)
    {
        static bool isOpen = true;
        ImGui::Begin("Components", &isOpen, ImGuiWindowFlags_HorizontalScrollbar);
        {
            if (ent != nullptr)
            {
                mEntity = ent;
                static bool showCreate;
                if (!ent->hasComponent<TileMap>())
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
                    if (ImGui::Button(ICON_FA_PLUS_CIRCLE " Add Component##createComp"))
                    {
                        showCreate = true;
                    }
                    showCreateComp(showCreate);
                    ImGui::PopStyleColor();
                    ImGui::Separator();
                }
                drawAnimate();
                drawTransform();
                drawSprite();
                drawTileMap();
            }
        }
        ImGui::End();
    }

    void ComponentPanel::render(SpriteBatch2D *renderer, glm::vec2 mcoords)
    {
        mTileMapPanel.renderTiles(renderer, mcoords);
    }

    void ComponentPanel::drawAnimate()
    {
        if (mEntity->hasComponent<Animate>())
        {
            if (ImGui::CollapsingHeader("Animate##comp"))
            {
                auto animate = mEntity->getComponent<Animate>();
                auto animations = animate->animations;
                static bool newAnin = false;

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
                if (ImGui::Button(ICON_FA_PLUS_CIRCLE "##add-anin"))
                {
                    newAnin = true;
                }
                ImGui::PopStyleColor();
                ImGui::SameLine();
                auto it = animations.begin();
                static std::string selected = it != animations.end() ? it->first : "";
                ImGui::PushItemWidth(100);
                if (ImGui::ComboBox("Animations", animations, selected))
                {
                }
                ImGui::PopItemWidth();
            }
        }
    }

    void ComponentPanel::drawTransform()
    {
        if (mEntity->hasComponent<Transform>())
        {
            if (ImGui::CollapsingHeader("Transform##comp"))
            {
                auto trans = mEntity->getComponent<Transform>();
                int position[] = {static_cast<int>(trans->position.x), static_cast<int>(trans->position.y)};

                ImGui::PushItemWidth(60);
                if (ImGui::DragInt2("Position X Y", position))
                {
                    trans->position.x = static_cast<float>(position[0]);
                    trans->position.y = static_cast<float>(position[1]);
                }
                int size[] = {static_cast<int>(trans->size.x), static_cast<int>(trans->size.y)};
                if (ImGui::DragInt2("Size W H", size))
                {
                    trans->size.x = size[0];
                    trans->size.y = size[1];
                }
                ImGui::PopItemWidth();
            }
        }
    }

    void ComponentPanel::drawSprite()
    {
        if (mEntity->hasComponent<Sprite>())
        {
            if (ImGui::CollapsingHeader("Sprite##comp"))
            {
                auto sprite = mEntity->getComponent<Sprite>();
                auto tilesets = mAManager->getTilesets();

                static std::string selected = sprite->mTextureId;

                static int sc = 100;
                static float scale = 1.0f;
                ImGui::PushItemWidth(100);
                {
                    ImGui::ComboBox<TileSet>("TileSheet", tilesets, selected);
                    ImGui::SameLine();
                    if (ImGui::InputInt("Scale", &sc, 5))
                    {
                        sc = CHECKLIMIT(sc, 25, 300);
                        scale = sc / 100.0f;
                    }
                }
                auto tileset = &tilesets[selected];
                if (sprite->mTexId != tileset->texId && tileset->texId > 0)
                {
                    sprite->mTextureId = selected;
                    sprite->mTexId = tileset->texId;
                }

                static char text[120];
                snprintf(text, 120, "Texure Size W:%d H:%d", tileset->texWidth, tileset->texHeight);
                ImGui::Text(text);
                ImGui::Separator();
                if (tileset != nullptr)
                {
                    if (ImGui::BeginTabBar("##TabBar"))
                    {
                        if (ImGui::BeginTabItem("Tile Coords"))
                        {
                            drawCanvas(tileset, scale);
                            ImGui::EndTabItem();
                        }
                        if (ImGui::BeginTabItem("Sprite Sheet"))
                        {
                            drawTexCoords(tileset, scale);
                            ImGui::EndTabItem();
                        }
                        ImGui::EndTabBar();
                    }
                }
            }
        }
    }

    void ComponentPanel::drawTileMap()
    {
        if (mEntity->hasComponent<TileMap>())
        {
            mTileMapPanel.draw(mEntity->getComponent<TileMap>());
        }
    }

    void ComponentPanel::drawCanvas(TileSet *tileset, float scale)
    {
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        auto size = ImGui::GetContentRegionAvail();

        ImVec2 cvPos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
        ImVec2 cv_destStart(cvPos.x, cvPos.y);

        ImVec2 cvDestEnd(cvPos.x + tileset->texWidth * scale, cvPos.y + tileset->texHeight * scale);
        ImGui::Image((void *)tileset->texId, ImVec2(tileset->texWidth * scale, tileset->texHeight * scale));
        {
            auto color = IM_COL32(255, 255, 255, 100);
            if (tileset->texId)
            {
                drawList->AddRect(cvPos, cvDestEnd, color);
            }

            if (tileset->tileWidth && tileset->tileHeight)
            {
                float tileWidth = tileset->tileWidth * scale;
                float tileHeight = tileset->tileHeight * scale;

                float textureHeight = tileset->texHeight * scale;
                float textureWidth = tileset->texWidth * scale;
                int columns = static_cast<int>(textureWidth / tileWidth);
                if (tileWidth)
                {
                    for (float y = 0; y < textureHeight; y += tileHeight)
                    {
                        drawList->AddLine(ImVec2(cvPos.x, cvPos.y + y),
                                          ImVec2(cvDestEnd.x, cvPos.y + y), color, 1.0f);
                    }
                }
                if (tileWidth)
                {
                    for (float x = 0; x < textureWidth; x += tileWidth)
                    {
                        drawList->AddLine(ImVec2(cvPos.x + x, cvPos.y),
                                          ImVec2(cvPos.x + x, cvDestEnd.y), color, 1.0f);
                    }
                }
                //Rect
                static bool mDown = false;
                if (ImGui::IsItemHovered())
                {
                    ImVec2 mpos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - cvPos.x, ImGui::GetIO().MousePos.y - cvPos.y);

                    float x = floor(mpos_in_canvas.x / tileWidth);
                    float y = floor(mpos_in_canvas.y / tileHeight);
                    ImVec2 start(x * tileWidth + cvPos.x, y * tileHeight + cvPos.y);
                    ImVec2 end(start.x + tileWidth, start.y + tileHeight);

                    drawList->AddRect(start, end, IM_COL32(255, 0, 0, 255));

                    if (mInputManager->onKeyPressed(SDL_BUTTON_LEFT))
                    {
                        mDown = true;
                        mSelectedtiles.clear();
                    }

                    if (!mInputManager->onKeyDown(SDL_BUTTON_LEFT))
                    {
                        mDown = false;
                    }

                    if (mDown)
                    {
                        ImVec2 vec(x, y);

                        auto found = std::find_if(mSelectedtiles.begin(), mSelectedtiles.end(),
                                                  [vec](const ImVec2 &m) -> bool { return m.x == vec.x && m.y == vec.y; });
                        if (found == mSelectedtiles.end())
                        {
                            mSelectedtiles.push_back(vec);
                        }
                    }
                }

                for (int i = 0; i < mSelectedtiles.size(); i++)
                {
                    ImVec2 start(mSelectedtiles[i].x * tileWidth + cv_destStart.x, mSelectedtiles[i].y * tileHeight + cv_destStart.y);
                    ImVec2 end(start.x + tileWidth, start.y + tileHeight);
                    drawList->AddRectFilled(start, end, IM_COL32(0, 255, 255, 50));
                }
            }
        }
    }

    void ComponentPanel::drawTexCoords(TileSet *tileset, float scale)
    {
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        static ImVector<ImVec2> points;
        if (ImGui::Button("Add Selection"))
        {
            // LOG_I("Selection Added\n");
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear Selection"))
        {
            points.clear();
        }
        ImVec2 cvPos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
        ImVec2 cv_destStart(cvPos.x, cvPos.y);
        ImVec2 cvDestEnd(cvPos.x + tileset->texWidth * scale, cvPos.y + tileset->texHeight * scale);
        ImGui::Image((void *)tileset->texId, ImVec2(tileset->texWidth * scale, tileset->texHeight * scale));
        {
            static ImVec2 StartCoords(0, 0);
            static ImVec2 EndCoords(0, 0);
            static bool firstClick = false;
            static bool SecondClick = false;

            static bool mDown;
            drawList->AddRectFilledMultiColor(cvPos, cvDestEnd, color1, color2, color3, color2);

            auto color = IM_COL32(255, 255, 255, 100);
            if (tileset->texId)
            {
                drawList->AddImage((void *)tileset->texId, cvPos, cvDestEnd);
                drawList->AddRect(cvPos, cvDestEnd, color);
            }
            if (ImGui::IsItemHovered())
            {
                ImVec2 mpos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - cvPos.x, ImGui::GetIO().MousePos.y - cvPos.y);

                if (mInputManager->onKeyDown(SDL_BUTTON_LEFT))
                {
                    if (!mDown)
                    {
                        StartCoords = ImVec2(mpos_in_canvas.x, mpos_in_canvas.y);
                        mDown = true;
                    }
                    drawList->AddRect(ImVec2(cvPos.x + StartCoords.x, cvPos.y + StartCoords.y), ImVec2(cvPos.x + mpos_in_canvas.x, cvPos.y + mpos_in_canvas.y), IM_COL32(255, 255, 255, 255));
                }

                if (!mInputManager->onKeyDown(SDL_BUTTON_LEFT))
                {
                    if (mDown)
                    {
                        EndCoords = ImVec2(mpos_in_canvas.x, mpos_in_canvas.y);
                        points.push_back(StartCoords);
                        points.push_back(EndCoords);
                        mDown = false;
                    }
                }

                if (mInputManager->onKeyPressed(SDL_BUTTON_RIGHT))
                {
                    if (points.size())
                    {
                        points.pop_back();
                        points.pop_back();
                    }
                }
            }
            else
            {
                mDown = false;
            }

            for (int i = 0; i < points.Size - 1; i += 2)
                drawList->AddRect(
                    ImVec2(cvPos.x + points[i].x, cvPos.y + points[i].y),
                    ImVec2(cvPos.x + points[i + 1].x, cvPos.y + points[i + 1].y),
                    IM_COL32(255, 255, 255, 255));
        }
    }

    bool ComponentPanel::showCreateComp(bool &open)
    {

        std::vector<std::string> datas = getCompList();

        bool save = false;
        if (open)
        {
            //Sprite Props
            auto tileMaps = mAManager->getTilesets();
            auto it = tileMaps.begin();
            static std::string selectedTex = it != tileMaps.end() ? it->first : "";

            // Transform Props
            static int tpos[] = {0, 0}, size[] = {0, 0};
            static float r = 0;

            static int selected = 0;
            auto pos = ImGui::GetWindowPos();
            ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y + 60));

            ImGui::SetNextWindowSize(ImVec2(250.0f, 300.0f));
            ImGui::OpenPopup("New Component");
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
            if (ImGui::BeginPopupModal("New Component", NULL, window_flags))
            {
                if (datas.size())
                {
                    ImGui::ComboBox("Type", datas, selected);
                    if (std::strcmp(datas[selected].c_str(), "Transform") == 0)
                    {
                        ImGui::DragInt2("Position##trans-modal", tpos, 1);
                        if (ImGui::DragInt2("Size##trans-modal", size, 1, 0))
                        {
                            size[0] = LIMIT(size[0], 0, 8000);
                            size[1] = LIMIT(size[1], 0, 8000);
                        }
                        if (ImGui::InputFloat("Rotation", &r))
                        {
                            r = LIMIT(r, 0.0f, 360.0f);
                        }
                    }
                }
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));

                if (ImGui::Button("Create##Comp"))
                {
                    open = false;
                    if (std::strcmp(datas[selected].c_str(), "Transform") == 0)
                    {
                        mEntity->addComponent<Transform>(tpos[1], tpos[0], size[0], size[1], r);
                    }
                    else if (std::strcmp(datas[selected].c_str(), "Sprite") == 0)
                    {
                        mEntity->addComponent<Sprite>("");
                    }
                    else if (std::strcmp(datas[selected].c_str(), "Animate") == 0)
                    {
                        mEntity->addComponent<Animate>();
                    }
                    else if (std::strcmp(datas[selected].c_str(), "TileMap") == 0)
                    {
                        mEntity->addComponent<TileMap>(32, 32);
                    }
                    save = true;
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel##modal-1"))
                    open = false;
                ImGui::PopStyleColor();
                ImGui::EndPopup();
            }
        }
        return save;
    }

    std::vector<std::string> ComponentPanel::getCompList()
    {
        std::vector<std::string> datas;
        if (!mEntity->hasComponent<Transform>())
        {
            datas.push_back("Transform");
        }
        if (!mEntity->hasComponent<Sprite>() && !mEntity->hasComponent<Animation>())
        {
            datas.push_back("Sprite");
        }
        if (!mEntity->hasComponent<Sprite>() && !mEntity->hasComponent<Animation>())
        {
            datas.push_back("Animate");
        }
        if (datas.size() == 3)
        {
            datas.push_back("TileMap");
        }
        return datas;
    }
} // namespace Plutus