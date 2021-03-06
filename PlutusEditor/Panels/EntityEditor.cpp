
#include "EntityEditor.h"
#include "SDL.h"
#include "ImGuiEx.h"
#include "Input/InputManager.h"
#include "Assets/AssetManager.h"
#include "Log/Logger.h"
#include "ECS/EntityManager.h"
#include "IconsFontAwesome5.h"

namespace Plutus
{
    void EntityEditor::init(EntityManager *emanager)
    {
        mEntManager = emanager;
        mComPanel.init();
    }

    void EntityEditor::draw()
    {
        drawEntity();
        mComPanel.drawUI();

        auto assetManager = Plutus::AssetManager::getInstance();
        auto mInputManager = InputManager::getInstance();

        auto data = assetManager->getTilesets();
        ImGuiStyle &style = ImGui::GetStyle();
        int oldSize = style.WindowMinSize.x;
        style.WindowMinSize.x = 300;
        ImGui::Begin("TileSets Window");

        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        static std::string selected = data.begin()->first;
        static bool mDown;
        {

            ImGui::RadioButton("Place", &mMode, EDIT_PLACE);
            ImGui::SameLine();
            ImGui::RadioButton("Select", &mMode, EDIT_SELECT);
            ImGui::SameLine();
            ImGui::RadioButton("Remove", &mMode, EDIT_REMOVE);

            auto tileTex = data[selected];

            static int sc = 100;
            static float scale = 1.0f;
            ImGui::PushItemWidth(100);
            {
                if (ImGui::InputInt("Scale", &sc, 5))
                {
                    sc = sc > 25 ? sc : 25;
                    scale = sc / 100.0f;
                }
                ImGui::SameLine();
                ImGui::ComboBox<TileSet>("TileSheet", data, selected);
                ImGui::PopItemWidth();
            }

            ImGui::Separator();
            //Show texture if size > 0
            if (tileTex.texWidth > 0 && tileTex.texHeight > 0)
            {
                ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!

                ImVec2 cv_destStart(canvas_pos.x, canvas_pos.y);
                ImVec2 cv_destEnd(canvas_pos.x + tileTex.texWidth * scale, canvas_pos.y + tileTex.texHeight * scale);

                //background
                draw_list->AddRectFilledMultiColor(canvas_pos, cv_destEnd, IM_COL32(50, 50, 50, 255),
                                                   IM_COL32(50, 50, 60, 255), IM_COL32(60, 60, 70, 255), IM_COL32(50, 50, 60, 255));

                ImGui::InvisibleButton("#inv", ImVec2(tileTex.texWidth * scale, tileTex.texHeight * scale));
                {
                    //Grid Line color
                    auto color = IM_COL32(255, 255, 255, 100);
                    if (tileTex.texId)
                    {
                        draw_list->AddImage((void *)tileTex.texId, canvas_pos, cv_destEnd);
                        draw_list->AddRect(canvas_pos, cv_destEnd, color);
                    }
                    if (tileTex.tileWidth && tileTex.tileHeight)
                    {
                        float tileWidth = tileTex.tileWidth * scale;
                        float tileHeight = tileTex.tileHeight * scale;

                        float textureHeight = tileTex.texHeight * scale;
                        float textureWidth = tileTex.texWidth * scale;
                        int columns = static_cast<int>(textureWidth / tileWidth);
                        if (tileWidth)
                        {

                            for (float y = 0; y < textureHeight; y += tileHeight)
                            {
                                draw_list->AddLine(ImVec2(canvas_pos.x, canvas_pos.y + y),
                                                   ImVec2(cv_destEnd.x, canvas_pos.y + y), color, 1.0f);
                            }
                        }
                        if (tileWidth)
                        {
                            for (float x = 0; x < textureWidth; x += tileWidth)
                            {
                                draw_list->AddLine(ImVec2(canvas_pos.x + x, canvas_pos.y),
                                                   ImVec2(canvas_pos.x + x, cv_destEnd.y), color, 1.0f);
                            }
                        }
                        //Rect
                        static bool mDown = false;
                        if (ImGui::IsItemHovered())
                        {
                            ImVec2 mpos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - canvas_pos.x, ImGui::GetIO().MousePos.y - canvas_pos.y);

                            float x = floor(mpos_in_canvas.x / tileWidth);
                            float y = floor(mpos_in_canvas.y / tileHeight);
                            ImVec2 start(x * tileWidth + canvas_pos.x, y * tileHeight + canvas_pos.y);
                            ImVec2 end(start.x + tileWidth, start.y + tileHeight);

                            draw_list->AddRect(start, end, IM_COL32(255, 0, 0, 255));

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
                                    LOG_I("X:{0}, Y:{1} {2} textCoord: {3} total: {4}", x, y, columns, x + y * columns, mSelectedtiles.size());
                                }
                            }
                        }

                        for (int i = 0; i < mSelectedtiles.size(); i++)
                        {
                            ImVec2 start(mSelectedtiles[i].x * tileWidth + cv_destStart.x, mSelectedtiles[i].y * tileHeight + cv_destStart.y);
                            ImVec2 end(start.x + tileWidth, start.y + tileHeight);
                            draw_list->AddRectFilled(start, end, IM_COL32(0, 255, 255, 50));
                        }
                    }
                }
            }
        }
        ImGui::End();

        style.WindowMinSize.x = oldSize;
    }

    void EntityEditor::drawEntity()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 300));
        ImGui::Begin("Scene Editor");
        {
            static auto mInManager = Plutus::InputManager::getInstance();
            auto layers = mEntManager->getLayers();
            auto layer = mEntManager->getCurrentLayer();

            ImGui::Text(ICON_FA_LAYER_GROUP " Layers");
            ImGui::Separator();
            static auto current = layer->name;

            static bool openLayerModal = false;
            if (ImGui::Button("Add New##layer"))
            {
                openLayerModal = true;
                modalPos = mInManager->getMouseCoords();
            }
            if (openLayerModal)
            {
                std::string newLayer = LayerModal("Create Layer", &openLayerModal);
                if (!openLayerModal && !newLayer.empty())
                {
                    mEntManager->addLayer(newLayer);
                    current = newLayer;
                }
            }

            ImGui::SameLine();
            ImGui::Button("Edit Name");
            ImGui::PushItemWidth(100);
            if (ImGui::ComboBox<Plutus::Layer>("Layers", *layers, current))
            {
                mEntManager->setCurrentLayer(current);
            }
            ImGui::Checkbox("IsVisible", &layer->isVisible);
            ImGui::Separator();
            static bool openEntModal = false;
            if (ImGui::Button("Add New##ent"))
            {
                openEntModal = true;
                modalPos = mInManager->getMouseCoords();
            }

            if (openEntModal)
            {
                std::string newEntity = LayerModal("Create Entity", &openEntModal);
                if (!openEntModal && !newEntity.empty())
                {
                    mCurrentEnt = mEntManager->addEntity(newEntity);
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Remove##ent"))
            {
            }
            /***************************Entity List*************************/
            ImGui::Separator();
            ImGui::Text("Entity List");
            ImGui::Separator();
            static int selected = 0;
            if (ImGui::Entities("Entities##list", layer->entities, selected))
            {
                if (layer->entities.size() && layer->entities[selected])
                {
                    mCurrentEnt = layer->entities[selected];
                }
            }
            if (mCurrentEnt)
            {
                mComPanel.setEntity(mCurrentEnt);
            }
        }
        ImGui::End();
        ImGui::PopStyleVar(1);
    }

    std::string EntityEditor::LayerModal(char *label, bool *open)
    {
        std::string result = "";
        if (*open)
        {
            ImGui::SetNextWindowPos(ImVec2(modalPos.x + 300, modalPos.y + 170));
            ImGui::SetNextWindowSize(ImVec2(260.0f, 90.0f));

            ImGui::OpenPopup(label);
            static char newlayer[128];
            if (ImGui::BeginPopupModal(label, NULL))
            {
                ImGui::InputText("Name##modal-1 ", newlayer, IM_ARRAYSIZE(newlayer));

                if (ImGui::Button("Save##modal-1") && !std::string(newlayer).empty())
                {
                    result = newlayer;
                    *open = false;
                }

                ImGui::SameLine();
                if (ImGui::Button("Close##modal-1"))
                    *open = false;

                ImGui::EndPopup();
            }

            if (mShowCreateLayer)
                ImGui::CloseCurrentPopup();
        }
        return result;
    }
} // namespace Plutus