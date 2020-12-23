#include "LayerEditor.h"
#include "imgui.h"
#include "ECS/EntityManager.h"
#include "Input/InputManager.h"
#include "ImGuiEx.h"

namespace Plutus
{

    void LayerEditor::LayerControls()
    {
        auto mEntManager = EntityManager::getInstance();
        auto mInManager = InputManager::getInstance();
        ImGui::Begin("Layer Control");
        ImGui::Text("Layers");
        ImGui::Separator();
        static bool showAddDialog = false;
        static glm::vec2 pos;
        if (ImGui::Button("Add New"))
        {
            showAddDialog = true;

            pos = mInManager->getMouseCoords();
        }
        ImGui::SameLine();
        ImGui::Button("Edit Name");
        if (showAddDialog)
        {
            ImGui::SetNextWindowPos(ImVec2(pos.x + 300, pos.y + 170));
            ImGui::SetNextWindowSize(ImVec2(260.0f, 90.0f));
            if (LayerModal())
            {
                showAddDialog = false;
            }
        }
        ImGui::PushItemWidth(100);
        static int selected = 0;
        auto layers = mEntManager->getLayers();
        if (ImGui::ComboBox("Layers", layers, selected))
        {
            mEntManager->setLayer(layers[selected]->name);
        }
        ImGui::SameLine();
        ImGui::Checkbox("Visible", &layers[selected]->isVisible);

        ImGui::PopItemWidth();

        ImGui::Text("Layer Entities");
        ImGui::Separator();
        const char *listbox_items[] = {"Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon"};
        static int listbox_item_current = 1;
        ImGui::ListBox("", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

        ImGui::End();
    }

    bool LayerEditor::LayerModal()
    {
        auto mEntManager = EntityManager::getInstance();
        bool shouldClose = false;
        ImGui::OpenPopup("New Layer");
        static char newlayer[128];
        if (ImGui::BeginPopupModal("New Layer", NULL))
        {
            ImGui::InputText("Name", newlayer, IM_ARRAYSIZE(newlayer));
            if (ImGui::Button("Save"))
            {
                mEntManager->addLayer(newlayer);
                shouldClose = true;
            }

            ImGui::SameLine();
            if (ImGui::Button("Close"))
                shouldClose = true;

            ImGui::EndPopup();
        }

        if (shouldClose)
            ImGui::CloseCurrentPopup();
        return shouldClose;
    }
} // namespace Plutus