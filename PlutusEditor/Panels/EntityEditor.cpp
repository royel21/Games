
#include "EntityEditor.h"
#include "SDL.h"
#include "ImGuiEx.h"
#include "Input/InputManager.h"
#include "Assets/AssetManager.h"
#include "Log/Logger.h"
#include "ECS/EntityManager.h"
#include "IconsFontAwesome5.h"
#include "ECS/SceneLoader.h"
#include "Utils.h"
#include "EditorUI.h"

namespace Plutus
{
    void EntityEditor::init(EntityManager *emanager, EditorUI *parent)
    {
        mParentUI = parent;
        mInputManager = Plutus::InputManager::getInstance();
        mEntManager = emanager;
        mLayers = mEntManager->getLayers();
        mCurrentLayer = mEntManager->getCurrentLayer();

        mCurLayerName = mCurrentLayer->name;

        if (mCurrentLayer != nullptr && mCurrentLayer->entities.size() > 0)
        {
            mParentUI->setEntity(mCurrentLayer->entities[0]);
        }
    }

    void EntityEditor::drawTileset()
    {
    }

    void EntityEditor::draw()
    {
        ImGui::Begin("Scene Editor");
        layers();
        entity();
        ImGui::End();
    }

    void EntityEditor::layers()
    {
        ImGui::Text(ICON_FA_LAYER_GROUP " Layers");
        ImGui::SameLine();
        static bool openLayerModal = false;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
        if (ImGui::Button(ICON_FA_PLUS_CIRCLE " ##layer"))
        {
            openLayerModal = true;
        }
        ImGui::PopStyleColor();
        if (openLayerModal)
        {
            auto pos = ImGui::GetWindowPos();
            ImGui::SetNextWindowPos(ImVec2(pos.x + 100, pos.y + 60));
            std::string newLayer = LayerModal("New Layer", &openLayerModal);
            if (!openLayerModal && !newLayer.empty())
            {
                mCurrentLayer = mEntManager->addLayer(newLayer);
                mCurLayerName = newLayer;
                mParentUI->setEntity(nullptr);
            }
        }

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
        ImGui::Button(ICON_FA_EDIT " ##Edit_Layer_Name");
        //Remove Layer from list
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_TRASH_ALT " ##Remove_Layer"))
        {
            if (mEntManager->removeLayer(mCurrentLayer->name))
            {
                mLayers = mEntManager->getLayers();
                if (mLayers->size())
                {
                    auto it = mLayers->begin();
                    mEntManager->setCurrentLayer(it->first);
                    mCurrentLayer = mEntManager->getCurrentLayer();
                    mCurLayerName = mCurrentLayer->name;

                    auto ent = mCurrentLayer->entities.begin();
                    mParentUI->setEntity(ent != mCurrentLayer->entities.end() ? *ent : nullptr);
                }
                else
                {
                    mParentUI->setEntity(nullptr);
                    mCurLayerName = "";
                }
            }
        }
        ImGui::PopStyleColor();
        ImGui::PushItemWidth(80);
        if (ImGui::ComboBox<Layer>("Layers", *mLayers, mCurLayerName))
        {
            mEntManager->setCurrentLayer(mCurLayerName);
            mCurrentLayer = mEntManager->getCurrentLayer();
            if (mCurrentLayer->entities.size() > 0)
            {
                mParentUI->setEntity(mCurrentLayer->entities[0]);
            }
        }
        ImGui::SameLine();
        ImGui::Checkbox(ICON_FA_EYE " ##IsVisible", &mCurrentLayer->isVisible);
    }

    /***************************Entity List*************************/
    void EntityEditor::entity()
    {
        ImGui::Separator();
        ImGui::Text(ICON_FA_LIST_OL " Entities");
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0.0f));
        static bool openEntModal = false;
        if (ImGui::Button(ICON_FA_PLUS_CIRCLE " ##ent"))
        {
            openEntModal = true;
        }
        if (openEntModal)
        {
            auto pos = ImGui::GetWindowPos();
            ImGui::SetNextWindowPos(ImVec2(pos.x + 100, pos.y + 120));
            std::string newEntity = LayerModal("New Entity", &openEntModal);
            if (!openEntModal && !newEntity.empty())
            {
                mParentUI->setEntity(mEntManager->addEntity(newEntity));
            }
        }
        ImGui::PopStyleColor();
        ImGui::Separator();
        static int selected = 0;
        static int remove = -1;
        if (ImGui::Entities("Entities##list", mCurrentLayer->entities, selected, remove))
        {
            if (mCurrentLayer->entities.size() && mCurrentLayer->entities[selected])
            {
                mParentUI->setEntity(mCurrentLayer->entities[selected]);
            }
        }
        if (remove > -1)
        {
            mEntManager->removeEntity(mCurrentLayer->entities[remove]);
            if (mCurrentLayer->entities.size() == 0)
            {
                mParentUI->setEntity(nullptr);
            }
            else
            {
                mParentUI->setEntity(mCurrentLayer->entities[0]);
            }
            remove = -1;
        }
    }

    std::string EntityEditor::LayerModal(char *label, bool *open)
    {
        std::string result = "";
        if (*open)
        {
            ImGui::SetNextWindowSize(ImVec2(260.0f, 95.0f));

            ImGui::OpenPopup(label);
            static char newlayer[128];
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
            if (ImGui::BeginPopupModal(label, NULL, window_flags))
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.33f, 0.33f, 0.33f, 0.8f));
                ImGui::InputText("Name##modal-1 ", newlayer, IM_ARRAYSIZE(newlayer));

                if (ImGui::Button("Save##modal-1") && !std::string(newlayer).empty())
                {
                    result = newlayer;
                    *open = false;
                }

                ImGui::SameLine();
                if (ImGui::Button("Close##modal-1"))
                    *open = false;

                ImGui::PopStyleColor(1);
                ImGui::EndPopup();
            }
        }
        return result;
    }

    void EntityEditor::loadScene(std::string path)
    {
        if (!path.empty() || Utils::windowDialog(OPEN_FILE, path))
        {
            if (Plutus::SceneLoader::loadFromJson(path.c_str(), mEntManager))
            {
                mCurrentLayer = mEntManager->getCurrentLayer();
                mCurLayerName = mCurrentLayer->name;
                if (mCurrentLayer != nullptr && mCurrentLayer->entities.size() > 0)
                {
                    mParentUI->setEntity(mCurrentLayer->entities[0]);
                    mParentUI->addRecent(path);
                }
            }
        }
    }
} // namespace Plutus