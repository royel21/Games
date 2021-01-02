
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
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 300));
        ImGui::Begin("Scene Editor");
        layers();
        entity();
        ImGui::End();
        ImGui::PopStyleVar(1);
    }

    void EntityEditor::layers()
    {
        ImGui::Text(ICON_FA_LAYER_GROUP " Layers");
        ImGui::Separator();
        static bool openLayerModal = false;

        if (ImGui::Button("Add New##layer"))
        {
            openLayerModal = true;
            modalPos = mInputManager->getMouseCoords();
        }
        if (openLayerModal)
        {
            std::string newLayer = LayerModal("Create Layer", &openLayerModal);
            if (!openLayerModal && !newLayer.empty())
            {
                mCurrentLayer = mEntManager->addLayer(newLayer);
                mCurLayerName = newLayer;
                mParentUI->setEntity(nullptr);
            }
        }

        ImGui::SameLine();
        ImGui::Button("Edit Name");
        ImGui::PushItemWidth(100);
        if (ImGui::ComboBox<Layer>("Layers", *mLayers, mCurLayerName))
        {
            mEntManager->setCurrentLayer(mCurLayerName);
            mCurrentLayer = mEntManager->getCurrentLayer();
            if (mCurrentLayer->entities.size() > 0)
            {
                mParentUI->setEntity(mCurrentLayer->entities[0]);
            }
        }
        ImGui::Checkbox("IsVisible", &mCurrentLayer->isVisible);
        ImGui::Separator();
    }

    /***************************Entity List*************************/
    void EntityEditor::entity()
    {
        static bool openEntModal = false;
        if (ImGui::Button("Add New##ent"))
        {
            openEntModal = true;
            modalPos = mInputManager->getMouseCoords();
        }

        if (openEntModal)
        {
            std::string newEntity = LayerModal("Create Entity", &openEntModal);
            if (!openEntModal && !newEntity.empty())
            {
                mParentUI->setEntity(mEntManager->addEntity(newEntity));
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Remove##ent"))
        {
        }

        ImGui::Separator();
        ImGui::Text(ICON_FA_LIST_OL " Entity List");
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
            std::cout << "Remove Ent:" << mCurrentLayer->entities[remove]->name << std::endl;
            mEntManager->removeEntity(mCurrentLayer->entities[remove]);
            if (mCurrentLayer->entities.size() == 0)
                mParentUI->setEntity(nullptr);
            remove = -1;
        }
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

    void EntityEditor::loadScene()
    {
        std::string path;
        if (Utils::windowDialog(OPEN_FILE, path))
        {
            if (Plutus::SceneLoader::loadFromJson(path.c_str(), mEntManager))
            {
                mCurrentLayer = mEntManager->getCurrentLayer();
                if (mCurrentLayer != nullptr && mCurrentLayer->entities.size() > 0)
                {
                    mParentUI->setEntity(mCurrentLayer->entities[0]);
                }
            }
        }
    }
} // namespace Plutus