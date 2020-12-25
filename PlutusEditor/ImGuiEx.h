#pragma once
#include <vector>
#include "imgui.h"
#include "iostream"
#include <map>
#include "entt.hpp"

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
    inline bool ComboBox(const char *label, const std::map<std::string, T> &data, std::string &selected)
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

    bool ListBox(const std::string &label, std::vector<entt::entity> data, entt::entity &selected)
    {
        bool isSelected = false;
        if (ImGui::ListBoxHeader("List", data.size()))
        {
            for (size_t i = 0; i < data.size(); i++)
            {
                
                bool is_selected = data[i] == selected;
                
                if (ImGui::Selectable(m.first.c_str(), is_selected))
                {
                    isSelected = true;
                    selected = m.first;
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
        }
    }
} // namespace ImGui