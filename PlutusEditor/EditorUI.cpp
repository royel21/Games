#include "EditorUI.h"
#include "Log/Logger.h"
#include "Graphics/Camera2D.h"
#include "ImGuiEx.h"
#include "LayerEditor.h"

#define mapIn(x, min_in, max_in, min_out, max_out) (x - min_in) * (max_out - min_out) / (max_in - min_in) + min_out

#define CHECKLIMIT(val, min, max) val<min ? min : val> max ? max : val

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

	EditorUI *EditorUI::getInstance(Window *_window, Camera2D *cam)
	{
		if (!mInstance)
		{
			mInstance = new EditorUI();
			mInstance->Init(_window, cam);
		}

		return mInstance;
	}

	void EditorUI::Init(Window *win, Camera2D *cam)
	{
		mFb.init(300, 300);
		mCamera = cam;
		mWindow = win;

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
		mImGui_IO->FontDefault = mImGui_IO->Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);
		mDebugRender = Plutus::DebugRender::geInstances();
		mDebugRender->init(win, cam);
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

	void EditorUI::DrawUI()
	{
		beginUI();
		drawMainDockingWin();
		viewPort();
		viewPortControl();
		mEntityEditor.draw();
		showDemo();
		LayerEditor::LayerControls();
		endUI();
	}

	void EditorUI::viewPort()
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		static bool open = true;
		ImGui::Begin("Viewport", &open, flags);
		auto size = ImGui::GetContentRegionAvail();

		if (size.x != mViewportSize.x || size.y != mViewportSize.y)
		{
			mViewportSize = size;
			mFb.resize(mViewportSize.x, mViewportSize.y);
		}

		ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!

		int xPos = static_cast<int>(ImGui::GetIO().MousePos.x - ImGui::GetCursorScreenPos().x - ImGui::GetScrollX());
		int yPos = static_cast<int>(ImGui::GetIO().MousePos.y - ImGui::GetCursorScreenPos().y - ImGui::GetScrollY());

		auto scaleScreen = mCamera->getScaleScreen();

		glm::vec2 pos;
		auto cvPortSize = mCamera->getViewPortSize();
		pos.x = mapIn(xPos, 0, static_cast<int>(size.x), 0, cvPortSize.x);
		pos.y = mapIn(yPos, 0, static_cast<int>(size.y), 0, cvPortSize.y);

		auto sqrPos = mDebugRender->getSquareCoords(pos);

		ImGui::Image(reinterpret_cast<void *>(mFb.getTextureId()), size, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar(1);
	}

	void EditorUI::onEvent(SDL_Event &event)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	void EditorUI::viewPortControl()
	{
		ImGui::Begin("ViewPort");
		static bool moveCamera = false;
		if (ImGui::CollapsingHeader("Camera Controls"))
		{
			float scale = mCamera->getScale();
			if (ImGui::InputFloat("Zoom", &scale, 0.05f, 0.1f, 2))
			{
				scale = CHECKLIMIT(scale, 0.4, 6);
				mCamera->setScale(scale);
			}
			if (ImGui::Button("Go to center"))
			{
				mCamera->setPosition(glm::vec2(0, 0));
			}
			ImGui::Checkbox("Move Camera", &moveCamera);

			static float bg[] = {mVPColor.x, mVPColor.y, mVPColor.z, mVPColor.w};
			if (ImGui::ColorEdit4("ViewPort BG", bg, ImGuiColorEditFlags_AlphaBar))
			{
				mVPColor = glm::vec4(bg[0], bg[1], bg[2], bg[3]);
			}
		}
		if (ImGui::CollapsingHeader("Grid Controls"))
		{
			ImGui::Text("Grid Controls");
			static bool showGrid = true;
			if (ImGui::Checkbox("Enable", &showGrid))
			{
				mDebugRender->setShouldDraw(showGrid);
			}
			auto cellSize = mDebugRender->getCellSize();

			static int cellWidth = cellSize.x;
			if (ImGui::InputInt("Width", &cellWidth, 2))
			{
				cellWidth = CHECKLIMIT(cellWidth, 0, 200);
			}
			static int cellHeight = cellSize.y;
			if (ImGui::InputInt("Height", &cellHeight, 2))
			{
				cellHeight = CHECKLIMIT(cellHeight, 0, 200);
			}
			static float color[] = {0, 0, 0, 1.0f};
			if (ImGui::ColorEdit3("Grid Color", color))
			{
				LOG_I("{0} {1} {2} {3}", color[0] * 255, color[1] * 255, color[1] * 255, 1);
				mDebugRender->setColor(ColorRGBA8(color[0] * 255, color[1] * 255, color[1] * 255, 255));
			}
			mDebugRender->setCellSize(cellWidth, cellHeight);
		}
		ImGui::End();

		if (moveCamera)
		{
			if (mInputManager->onKeyPressed(SDL_BUTTON_LEFT))
			{
				setLastCoord(mInputManager->getMouseCoords());
			}
			// move the camera
			if (mInputManager->onKeyDown(SDL_BUTTON_LEFT))
			{
				//get the mouseCoord;
				auto coords = mInputManager->getMouseCoords();
				// get mouse last coords
				auto last = getLastCoords();
				// get the camera scale factor
				float scale = mCamera->getScale();
				//calculate distance move with scale
				auto result = (coords - last) / scale;
				//get the camera position
				auto camPos = mCamera->getPosition();

				glm::vec2 newPos(camPos.x - result.x, result.y + camPos.y);
				mCamera->setPosition(newPos);
				setLastCoord(coords);
			}
		}
		//Control the camera with CTRL + MOUSE WHEEL
		if (mInputManager->onKeyDown(SDLK_LCTRL))
		{
			float scale = mCamera->getScale();

			if (mInputManager->getMouseWheel() > 0 && scale < 5)
			{
				mCamera->setScale(scale + 0.05);
			}
			else if (mInputManager->getMouseWheel() < 0 && scale > 0.5)
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
		else
		{
			// ShowDockingDisabledMessage();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Docking"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
					dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
					dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
					dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))
					dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
					dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
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

} // namespace Plutus

// void EditorUI::drawTilesetEditor()
// {
// 	auto assetManager = Plutus::AssetManager::getInstance();
// 	auto data = assetManager->getTilesets();
// 	ImGuiStyle &style = ImGui::GetStyle();
// 	int oldSize = style.WindowMinSize.x;
// 	style.WindowMinSize.x = 300;
// 	ImGui::Begin("TileSets Window");

// 	if (data.size() == 0)
// 	{
// 		ImGui::End();
// 	}
// 	ImDrawList *draw_list = ImGui::GetWindowDrawList();
// 	static int selected = 0;
// 	static bool mDown;

// 	{

// 		ImGui::RadioButton("Place", &m_mode, EDIT_PLACE);
// 		ImGui::SameLine();
// 		ImGui::RadioButton("Select", &m_mode, EDIT_SELECT);
// 		ImGui::SameLine();
// 		ImGui::RadioButton("Remove", &m_mode, EDIT_REMOVE);

// 		mTileTexture = AssetManager::getTexture(data[selected]);

// 		static int sc = 100;
// 		static float scale = 1.0f;
// 		ImGui::PushItemWidth(100);
// 		{
// 			if (ImGui::InputInt("Scale", &sc, 5))
// 			{
// 				sc = sc > 25 ? sc : 25;
// 				scale = sc / 100.0f;
// 			}
// 			ImGui::SameLine();
// 			ImGui::ComboBox("TileSheet", data, selected);
// 			ImGui::PopItemWidth();
// 		}

// 		ImGui::Separator();

// 		ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!

// 		ImVec2 cv_destStart(canvas_pos.x, canvas_pos.y);
// 		ImVec2 cv_destEnd(canvas_pos.x + mTileTexture.width * scale, canvas_pos.y + mTileTexture.height * scale);

// 		auto color = IM_COL32(255, 255, 255, 100);
// 		//background
// 		draw_list->AddRectFilledMultiColor(canvas_pos, cv_destEnd, IM_COL32(50, 50, 50, 255), IM_COL32(50, 50, 60, 255), IM_COL32(60, 60, 70, 255), IM_COL32(50, 50, 60, 255));

// 		ImGui::InvisibleButton("inv", ImVec2(mTileTexture.width * scale, mTileTexture.height * scale));
// 		draw_list->AddImage((void *)mTileTexture.id, canvas_pos, cv_destEnd);
// 		draw_list->AddRect(canvas_pos, cv_destEnd, color);

// 		float tileWidth = data[selected]->width * scale;
// 		float tileHeight = data[selected]->height * scale;

// 		float textureHeight = mTileTexture.height * scale;
// 		float textureWidth = mTileTexture.width * scale;
// 		int columns = static_cast<int>(textureWidth / tileWidth);

// 		for (float y = 0; y < textureHeight; y += tileHeight)
// 		{
// 			draw_list->AddLine(ImVec2(canvas_pos.x, canvas_pos.y + y),
// 							   ImVec2(cv_destEnd.x, canvas_pos.y + y), color, 1.0f);
// 		}

// 		for (float x = 0; x < textureWidth; x += tileWidth)
// 		{
// 			draw_list->AddLine(ImVec2(canvas_pos.x + x, canvas_pos.y),
// 							   ImVec2(canvas_pos.x + x, cv_destEnd.y), color, 1.0f);
// 		}
// 		//Rect
// 		static bool mDown = false;
// 		if (ImGui::IsItemHovered())
// 		{
// 			ImVec2 mpos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - canvas_pos.x, ImGui::GetIO().MousePos.y - canvas_pos.y);

// 			float x = floor(mpos_in_canvas.x / tileWidth);
// 			float y = floor(mpos_in_canvas.y / tileHeight);
// 			ImVec2 start(x * tileWidth + canvas_pos.x, y * tileHeight + canvas_pos.y);
// 			ImVec2 end(start.x + tileWidth, start.y + tileHeight);

// 			draw_list->AddRect(start, end, IM_COL32(255, 0, 0, 255));

// 			if (mInputManager->onKeyPressed(SDL_BUTTON_LEFT))
// 			{
// 				mDown = true;
// 				Selectedtiles.clear();
// 			}

// 			if (!mInputManager->onKeyDown(SDL_BUTTON_LEFT))
// 			{
// 				mDown = false;
// 			}

// 			if (mDown)
// 			{
// 				ImVec2 vec(x, y);

// 				auto found = std::find_if(Selectedtiles.begin(), Selectedtiles.end(),
// 										  [vec](const ImVec2 &m) -> bool { return m.x == vec.x && m.y == vec.y; });
// 				if (found == Selectedtiles.end())
// 				{
// 					Selectedtiles.push_back(vec);
// 					// LOG_I("X:{0}, Y:{1} {2} textCoord: {3}", x, y, columns, x + y * columns);
// 				}
// 			}
// 		}

// 		for (int i = 0; i < Selectedtiles.size(); i++)
// 		{
// 			ImVec2 start(Selectedtiles[i].x * tileWidth + cv_destStart.x, Selectedtiles[i].y * tileHeight + cv_destStart.y);
// 			ImVec2 end(start.x + tileWidth, start.y + tileHeight);
// 			draw_list->AddRectFilled(start, end, IM_COL32(0, 255, 255, 50));
// 		}
// 	}
// 	ImGui::End();
// 	style.WindowMinSize.x = oldSize;
// }

// void EditorUI::EntityEditor()
// {
// 	ImGui::Begin("Entity Editor");

// 	ImGui::Separator();
// 	ImGui::Text("Components");
// 	ImGui::Separator();

// 	static int selectedComp = 0;
// 	std::vector<std::string> components({"Animation", "Image", "Transform", "Input"});
// 	if (ImGui::ComboBox("Components", components, selectedComp))
// 	{
// 		LOG_I("selected {0}", components[selectedComp]);
// 	}
// 	ImGui::End();
// }