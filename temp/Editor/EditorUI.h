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
#include "FrameBuffer.h"
#include "Graphics/DebugRenderer.h"

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
		EntityManager *mEntManager = nullptr;
		glm::vec2 lastCoords;
		int m_mode = EDIT_PLACE;
		bool m_moveCamera = false;
		FrameBuffer mFb;
		ImVec2 mViewportSize;
		Plutus::DebugRender *mDebugRender = nullptr;

		int mapToView(int x, int in_min, int in_max, int out_min, int out_max)
		{
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}

	public:
		std::vector<ImVec2> Selectedtiles;

	public:
		static EditorUI *getInstance(Plutus::Window *_window, Camera2D *cam);

		~EditorUI();

		void DrawUI();
		void beginUI();
		void endUI();
		void destroy();
		//ImGui Panels
		void createTile();
		void drawTilesetEditor();
		void CameraControl();
		void tilesProps();
		void EntityEditor();
		void drawMainDockingWin();
		//Bind Framebuffer
		void bindFB();
		void unBindFB();
		//Un Bind Framebuffer
		void onEvent(SDL_Event &event);

		ImGuiIO *getIO() { return mImGui_IO; }

		const glm::vec2 &getLastCoords() { return lastCoords; }

		inline void setLastCoord(const glm::vec2 &coords) { lastCoords = coords; }
		inline bool moveCamera() { return m_moveCamera; }

		inline bool isHover() { return ImGui::IsAnyItemHovered() || ImGui::IsAnyWindowHovered(); }

		inline void showDemo() { ImGui::ShowDemoWindow(); }
		inline void setTileSheet(const std::string textureId)
		{
			m_texture.texture = Plutus::AssetManager::getTexture(textureId);
		}

	private:
		EditorUI();
		void viewPort();
		void Init(Plutus::Window *_window, Camera2D *cam);
	};

} // namespace Plutus