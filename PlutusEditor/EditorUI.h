#pragma once

#include <map>
#include <SDL.h>
#include <cstring>
#include <vector>

#include "Window.h"
#include "Input/InputManager.h"
#include "Assets/AssetManager.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "FrameBuffer.h"
#include "Graphics/DebugRenderer.h"
#include "Panels/EntityEditor.h"

namespace Plutus
{
	class Camera2D;

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
		InputManager *mInputManager = nullptr;
		GLTexture mTileTexture;
		TileSet m_texture;
		Camera2D *mCamera = nullptr;
		glm::vec2 lastCoords;
		bool m_moveCamera = false;
		FrameBuffer mFb;
		ImVec2 mViewportSize;
		DebugRender *mDebugRender = nullptr;
		AssetManager *mAssetsMangager;
		glm::vec4 mVPColor;
		//Panels
		EntityEditor mEntityEditor;

	public:
		std::vector<ImVec2>
			Selectedtiles;

	public:
		static EditorUI *getInstance(Plutus::Window *_window, Camera2D *cam);

		~EditorUI();

		void DrawUI();
		void beginUI();
		void endUI();
		void destroy();
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
			m_texture = mAssetsMangager->getTexture(textureId);
		}
		void viewPortBGColor(float r, float b, float g, float a);

	private:
		EditorUI();
		//ImGui Panels
		void drawMainDockingWin();
		void viewPort();
		void viewPortControl();

		void Init(Plutus::Window *_window, Camera2D *cam);
	};

} // namespace Plutus