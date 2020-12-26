#pragma once
#include <vector>
#include <unordered_map>
#include "imgui.h"
#include "iostream"
#include "entt.hpp"
#include "ECS/EntityManager.h"

namespace ImGui
{
    template <typename T>
    inline bool ComboBox(const char *label, const std::vector<T *> &data, int &selected)
    {
        bool isSelected = false;

        if (ImGui::BeginCombo(label, data[selected]->name.c_str()))
        {
            int i = 0;
            for (auto m : data)
            {
                bool is_selected = m->name.compare(data[selected]->name) == 0;
                if (ImGui::Selectable(m->name.c_str(), is_selected))
                {
                    isSelected = true;
                    selected = i;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                i++;
            }
            ImGui::EndCombo();
        }

        return isSelected;
    }

    inline bool ComboBox(const char *label, const std::vector<std::string> &data, int &selected)
    {
        bool isSelected = false;

        if (ImGui::BeginCombo(label, data[selected].c_str()))
        {
            int i = 0;
            for (auto m : data)
            {
                bool is_selected = m.compare(data[selected]) == 0;
                if (ImGui::Selectable(m.c_str(), is_selected))
                {
                    isSelected = true;
                    selected = i;
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                i++;
            }
            ImGui::EndCombo();
        }

        return isSelected;
    }
    template <typename T>
    inline bool ComboBox(const char *label, const std::unordered_map<std::string, T> &data, std::string &selected)
    {
        bool isSelected = false;

        if (ImGui::BeginCombo(label, selected.c_str()))
        {
            for (auto m : data)
            {
                bool is_selected = m.first.compare(selected) == 0;
                if (ImGui::Selectable(m.first.c_str(), is_selected))
                {
                    isSelected = true;
                    selected = m.first;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        return isSelected;
    }

    inline bool ListBox(const char *label, std::vector<Plutus::Entity *> data, Plutus::Entity *selected)
    {
        bool isSelected = false;
        if (ImGui::ListBoxHeader(label, data.size()))
        {
            for (auto ent : data)
            {
                bool is_selected = ent->name.compare(selected->name) == 0;

                if (ImGui::Selectable(ent->name.c_str(), is_selected))
                {
                    isSelected = true;
                    selected = ent;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::ListBoxFooter();
        }
        return isSelected;
    }

    inline bool Entities(const char *label, std::vector<Plutus::Entity *> entities, int &selected)
    {
        bool isSelected = false;
        for (int i = 0; i < entities.size(); i++)
        {
            bool is_selected = i == selected;

            if (ImGui::Selectable(entities[i]->name.c_str(), is_selected))
            {
                isSelected = true;
                selected = i;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        return isSelected;
    }
} // namespace ImGui