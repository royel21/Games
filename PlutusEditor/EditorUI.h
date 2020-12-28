#pragma once

#include <map>
#include <SDL.h>
#include <cstring>
#include <vector>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Panels/EntityEditor.h"
#include "glm/glm.hpp"
#include "FrameBuffer.h"
#include "Assets/AssetManager.h"

namespace Plutus
{
	class Camera2D;
	class EntityManager;
	class AssetManager;
	class DebugRender;
	class InputManager;
	class Window;

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

		GLTexture mTileTexture;
		Window *mWindow = nullptr;
		Camera2D *mCamera = nullptr;
		ImGuiIO *mImGui_IO = nullptr;
		DebugRender *mDebugRender = nullptr;
		InputManager *mInputManager = nullptr;
		FrameBuffer mFb;
		ImVec2 mViewportSize;
		AssetManager *mAssetsMangager;
		glm::vec4 mVPColor;
		//Panels
		EntityEditor mEntityEditor;
		EntityManager *mEntManager;

		bool mMoveCam = false;
		bool m_moveCamera = false;

		glm::vec2 lastCoords;

	public:
		std::vector<ImVec2>
			Selectedtiles;

	public:
		static EditorUI *getInstance();

		~EditorUI();

		void Init(Plutus::Window *_window, Camera2D *cam, EntityManager *emanager);

		void DrawUI();
		void beginUI();
		void endUI();
		void destroy();
		//Bind Framebuffer
		void bindFB();
		void unBindFB();
		void resizeFB(int w, int h);
		void resizeFB(glm::vec2 size);
		//Un Bind Framebuffer
		void onEvent(SDL_Event &event);

		ImGuiIO *getIO() { return mImGui_IO; }
		const glm::vec2 &getLastCoords() { return lastCoords; }

		inline void setLastCoord(const glm::vec2 &coords) { lastCoords = coords; }
		inline bool moveCamera() { return m_moveCamera; }
		inline bool isHover() { return ImGui::IsAnyItemHovered() || ImGui::IsAnyWindowHovered(); }
		inline void showDemo() { ImGui::ShowDemoWindow(); }
		void viewPortBGColor(float r, float b, float g, float a);
		void Serialize(Serializer &serializer);

	private:
		EditorUI();
		//ImGui Panels
		void drawMainDockingWin();
		void viewPort();
		void viewPortControl();
		void EntityList();
		void saveScene();
		void loadScene();
	};

} // namespace Plutus