#include "EditorUI.h"
#include "ImGuiEx.h"
#include "IconsFontAwesome5.h"

#include "Window.h"
#include "Graphics/DebugRenderer.h"
#include "Graphics/Camera2D.h"
#include "Serialize/Serialize.h"
#include "Input/InputManager.h"
#include "Log/Logger.h"
#include "ECS/EntityManager.h"

#include "LayerEditor.h"

#define mapIn(x, min_in, max_in, min_out, max_out) (x - min_in) * (max_out - min_out) / (max_in - min_in) + min_out

#define CHECKLIMIT(val, min, max) val<min ? min : val> max ? max : val
#define MAX_SCALE 1000
#define MIN_SCALE 10
#define SCALE_STEP 5

namespace Plutus
{
	EditorUI *EditorUI::mInstance = nullptr;

	EditorUI::EditorUI() : mVPColor(1, 1, 1, 1)
	{
		mInputManager = InputManager::getInstance();
	}
	EditorUI::~EditorUI()
	{
		destroy();
	}

	void EditorUI::destroy()
	{
		if (mImGui_IO)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplSDL2_Shutdown();
			ImGui::DestroyContext();
		}
	}

	EditorUI *EditorUI::getInstance()
	{
		if (!mInstance)
		{
			mInstance = new EditorUI();
		}

		return mInstance;
	}

	void EditorUI::Init(Window *win, Camera2D *cam, EntityManager *emanager)
	{
		mFb.resize(win->getScreenWidth(), win->getScreenHeight());
		mCamera = cam;
		mWindow = win;
		mEntManager = emanager;

		mAssetsMangager = AssetManager::getInstance();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		mImGui_IO = &ImGui::GetIO();
		mImGui_IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// Enable Docking
		mImGui_IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForOpenGL(mWindow->getSDLWindow(), mWindow->getGLContext());
		ImGui_ImplOpenGL3_Init("#version 130");

		ImGui::CaptureMouseFromApp();
		// mImGui_IO->FontDefault =
		mDebugRender = Plutus::DebugRender::geInstances();
		mDebugRender->init(win, cam);
		mEntityEditor.init(mEntManager);

		//Settup font

		// merge in icons from Font Awesome
		static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		mImGui_IO->Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);
		mImGui_IO->Fonts->AddFontFromFileTTF("assets/fonts/fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);
		// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid
		mImGui_IO->Fonts->AddFontDefault();
	}

	void EditorUI::beginUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(mWindow->getSDLWindow());
		ImGui::NewFrame();
	}

	void EditorUI::endUI()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (mImGui_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}
	}

	bool ZoomViewPort(int *value, int step, int min, int max)
	{
		bool zoom = false;
		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		ImGui::Text("Zoom");
		ImGui::SameLine();
		ImGui::PushItemWidth(60);
		if (ImGui::DragInt("##zoom-vp", value, step, min, max))
		{
			zoom = true;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine(0.0f, spacing);
		int tVal = *value;
		ImGui::PushButtonRepeat(true);
		if (ImGui::Button(ICON_FA_SEARCH_PLUS " ##zoom-vp"))
		{
			tVal += step;
			zoom = true;
		}
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button(ICON_FA_SEARCH_MINUS " ##zoom-vp"))
		{
			tVal -= step;
			zoom = true;
		}
		ImGui::SameLine(0.0f, spacing);
		if (ImGui::Button("Reset ##zoom-vp"))
		{
			tVal = 100;
			zoom = true;
		}
		ImGui::PopButtonRepeat();
		*value = CHECKLIMIT(tVal, min, max);
		return zoom;
	}

	void EditorUI::viewPortControl()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("ViewPort Controls");
		ImGui::Columns(2, NULL, true);
		ImGui::Text("Camera Control");
		{
			int zoom = static_cast<int>(round(mCamera->getScale() * 100));
			if (ZoomViewPort(&zoom, SCALE_STEP, MIN_SCALE, MAX_SCALE))
			{
				mCamera->setScale(zoom / 100.0f);
			}
			ImGui::Checkbox("Move Camera", &mMoveCam);
			ImGui::SameLine();

			if (ImGui::Button("Reset ##cam"))
			{
				mCamera->setPosition(glm::vec2(0, 0));
			}

			static float bg[] = {mVPColor.x, mVPColor.y, mVPColor.z, mVPColor.w};
			if (ImGui::ColorEdit4("VP BG", bg, ImGuiColorEditFlags_AlphaBar))
			{
				mVPColor = glm::vec4(bg[0], bg[1], bg[2], bg[3]);
			}
		}
		ImGui::NextColumn();
		ImGui::Text("Grid Controls");
		ImGui::Separator();
		{
			static bool showGrid = true;
			if (ImGui::Checkbox("Enable", &showGrid))
			{
				mDebugRender->setShouldDraw(showGrid);
			}

			auto cellc = mDebugRender->getCellCount();
			int cellCount[] = {cellc.x, cellc.y};
			if (ImGui::DragInt2("Count XY", cellCount))
			{
				cellc.x = CHECKLIMIT(cellCount[0], 0, 200);
				cellc.y = CHECKLIMIT(cellCount[1], 0, 200);
				mDebugRender->setCellCount(cellc.x, cellc.y);
			}

			auto cellS = mDebugRender->getCellSize();
			int cellSize[] = {cellS.x, cellS.y};
			if (ImGui::DragInt2("Cell Size", cellSize))
			{
				cellS.x = CHECKLIMIT(cellSize[0], 0, 200);
				cellS.y = CHECKLIMIT(cellSize[1], 0, 200);
			}
			mDebugRender->setCellSize(cellS.x, cellS.y);

			static float color[] = {0, 0, 0, 1.0f};
			if (ImGui::ColorEdit3("Grid Color", color))
			{
				LOG_I("{0} {1} {2} {3}", color[0] * 255, color[1] * 255, color[1] * 255, 1);
				mDebugRender->setColor(ColorRGBA8(color[0] * 255, color[1] * 255, color[1] * 255, 255));
			}

			ImGui::Separator();
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}
	void EditorUI::DrawUI()
	{
		beginUI();
		drawMainDockingWin();
		viewPort();
		viewPortControl();
		mEntityEditor.draw();
		// showDemo();
		LayerEditor::LayerControls();
		endUI();
	}

	void EditorUI::viewPort()
	{
		auto vsize = mFb.getSize();

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		static bool open = true;
		ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		ImGui::Begin("Viewport", &open, flags);
		auto size = ImGui::GetContentRegionAvail();

		auto winPos = ImGui::GetWindowSize();
		float x = max((winPos.x - vsize.x), 0) * 0.5;
		float y = max((winPos.y - vsize.y), 0) * 0.5;
		ImGui::SetCursorPos(ImVec2(x, y));

		ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!

		float xPos = ImGui::GetIO().MousePos.x - ImGui::GetCursorScreenPos().x;
		float yPos = ImGui::GetIO().MousePos.y - ImGui::GetCursorScreenPos().y;

		auto sqrPos = mDebugRender->getSquareCoords(glm::vec2(xPos, yPos));

		// LOG_I("{0} {1} {2} {3}", sqrPos.x, sqrPos.y, xPos, yPos);

		ImGui::Image(reinterpret_cast<void *>(mFb.getTextureId()), ImVec2(vsize.x, vsize.y), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0, 1.0, 1.0, 1.0), ImVec4(0.0, 0.0, 0.0, 1.0));
		if (ImGui::IsWindowHovered())
		{
			if (mMoveCam || mInputManager->onKeyDown(SDLK_LCTRL))
			{
				if (mInputManager->onKeyPressed(SDLK_r))
				{
					mCamera->setPosition(0, 0);
				}
				if (mInputManager->onKeyPressed(SDLK_z))
				{
					mCamera->setScale(1);
				}
				// move the camera
				if (mInputManager->onKeyPressed(SDL_BUTTON_LEFT))
				{
					lastCoords = mInputManager->getMouseCoords();
				}
				// move the camera
				if (mInputManager->onKeyDown(SDLK_LCTRL) && mInputManager->onKeyDown(SDL_BUTTON_LEFT))
				{
					//get the mouseCoord;
					auto coords = mInputManager->getMouseCoords();
					// get mouse last coords
					// get the camera scale factor
					float scale = mCamera->getScale();
					//calculate distance move with scale
					auto result = (coords - lastCoords) / scale;
					//get the camera position
					auto camPos = mCamera->getPosition();

					glm::vec2 newPos(camPos.x - result.x, result.y + camPos.y);
					mCamera->setPosition(newPos);
					lastCoords = coords;
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);
	}

	void EditorUI::onEvent(SDL_Event &event)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	void EditorUI::Serialize(Serializer &serializer)
	{
	}

	void EditorUI::EntityList()
	{
		//Control the camera with CTRL + MOUSE WHEEL
		if (mInputManager->onKeyDown(SDLK_LCTRL))
		{
			float scale = mCamera->getScale();

			if (mInputManager->getMouseWheel() > 0 && scale < 10.0f)
			{
				mCamera->setScale(scale + 0.05);
			}
			else if (mInputManager->getMouseWheel() < 0 && scale > 0.1f)
			{
				mCamera->setScale(scale - 0.05);
			}
		}
	}

	void EditorUI::drawMainDockingWin()
	{
		static bool isOpen;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport *viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &isOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO &io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Files"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Open", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
				{
				}
				if (ImGui::MenuItem("Save", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
				{
					saveScene();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	void EditorUI::bindFB()
	{
		mFb.bind();
		glColorMask(true, true, true, true);
		glClearColor(mVPColor.x, mVPColor.y, mVPColor.z, mVPColor.w);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void EditorUI::unBindFB()
	{
		mDebugRender->drawGrid();
		mFb.unBind();
	}

	void EditorUI::resizeFB(int w, int h)
	{
		auto fbSize = mFb.getSize();
		if (fbSize.x == w && fbSize.y != h)
		{
			mFb.resize(w, h);
		}
	}
	void EditorUI::resizeFB(glm::vec2 size)
	{
		resizeFB(size.x, size.y);
	}

	void EditorUI::saveScene()
	{
		Plutus::Serializer sr;
		mEntManager->serialize(sr);
		std::cout << sr.sb.GetString() << std::endl;
	}

} // namespace Plutus
