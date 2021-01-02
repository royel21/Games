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
        mEntity = ent;
        if (ent)
        {
            mEntity = ent;
        }
        static bool isOpen = true;
        ImGui::Begin("Components", &isOpen, ImGuiWindowFlags_HorizontalScrollbar);
        {
            if (mEntity)
            {
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
} // namespace Plutus