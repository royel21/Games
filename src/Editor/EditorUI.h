#pragma once

#include <map>
#include <SDL.h>
#include <cstring>
#include <vector>

#include "Window.h"
#include "Input/InputManager.h"
#include "Assets/AssetManager.h"
#include "Texture/TileSheet.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#define EDIT_PLACE 0
#define EDIT_SELECT 1
#define EDIT_REMOVE 2

namespace Plutus
{
	class Camera2D;
	class EntityManager;
	class EditorUI
	{
		struct props
		{
			bool flipX = false;
			bool flipY = false;
			int coord = 0;
		};

	private:
		static EditorUI *mInstance;
		ImGuiIO *mImGui_IO = nullptr;
		Window *mWindow = nullptr;
		InputManager *mIManager = nullptr;
		GLTexture mTileTexture;
		TileSheet m_texture;
		Camera2D *m_camera = nullptr;
		EntityManager *m_EManager = nullptr;
		int m_mode = EDIT_PLACE;
		bool m_moveCamera = false;

	public:
		std::vector<ImVec2> Selectedtiles;

	public:
		static EditorUI *getInstance(Plutus::Window *_window, Camera2D *cam, EntityManager *entityManager);

		~EditorUI();
		void beginUI();
		void DrawUI();
		void endUI();
		void onEvent(SDL_Event &event);
		void destroy();

		ImGuiIO *getIO() { return mImGui_IO; }
		inline void showDemo() { ImGui::ShowDemoWindow(); }
		void createTile();

		void tileset();

		void LayerControls();

		void tilesProps();
		bool LayerModal();

		inline bool moveCamera() { return m_moveCamera; }
		void setTileSheet(const std::string textureId) { m_texture.texture = Plutus::AssetManager::getTexture(textureId); }

		void EntityEditor();

		template <typename T>
		inline bool CustomComboBox(const char *label, const std::vector<T *> &data, int &selected)
		{
			ImGuiStyle &style = ImGui::GetStyle();
			ImGui::SameLine(0, style.ItemInnerSpacing.x);
			bool isSelected = false;

			if (ImGui::BeginCombo("##Maps", data[selected]->name.c_str()))
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
			ImGui::SameLine(0, style.ItemInnerSpacing.x);
			ImGui::Text(label);

			return isSelected;
		}

		bool isHover() { return ImGui::IsAnyItemHovered() || ImGui::IsAnyWindowHovered(); }

	private:
		EditorUI();

		void Init(Plutus::Window *_window, Camera2D *cam, EntityManager *entityManager);
	};

} // namespace Plutus

namespace ImGui
{
	inline bool ComboBox(const char *label, const std::vector<std::string> &data, int &selected)
	{
		ImGuiStyle &style = ImGui::GetStyle();
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		bool isSelected = false;

		if (ImGui::BeginCombo("##Maps", data[selected].c_str()))
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
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::Text(label);

		return isSelected;
	}
} // namespace ImGui