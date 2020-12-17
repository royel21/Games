#include "EditorUI.h"
#include "Log/Logger.h"
#include "Graphics/Camera2D.h"
#include "ECS/EntityManager.h"

#define CHECKLIMIT(val, min, max) val<min ? min : val> max ? max : val

namespace Plutus
{
	EditorUI *EditorUI::mInstance = nullptr;

	EditorUI::EditorUI()
	{
		mIManager = InputManager::getInstance();
	}

	EditorUI *EditorUI::getInstance(Window *_window, Camera2D *cam, EntityManager *entityManager)
	{
		if (!mInstance)
		{
			mInstance = new EditorUI();
			mInstance->Init(_window, cam, entityManager);
		}

		return mInstance;
	}

	EditorUI::~EditorUI()
	{
		destroy();
	}

	void EditorUI::Init(Window *_window, Camera2D *cam, EntityManager *entityManager)
	{
		m_camera = cam;
		mWindow = _window;
		m_EManager = entityManager;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		mImGui_IO = &ImGui::GetIO();
		mImGui_IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// Enable Docking
		mImGui_IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();
		ImGui_ImplSDL2_InitForOpenGL(mWindow->getSDLWindow(), mWindow->getGLContext());
		ImGui_ImplOpenGL3_Init("#version 130");

		ImGui::CaptureMouseFromApp();
	}

	void EditorUI::beginUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(mWindow->getSDLWindow());
		ImGui::NewFrame();
	}

	void EditorUI::DrawUI()
	{
		beginUI();
		beginWindow();
		endWindow();
		// tileset();

		// auto points = Selectedtiles;
		// if (points.size())
		// 	LOG_I("POINT: {0} {1} {2}", points[0].x, points[0].y, points.size());

		// LayerControls();
		// EntityEditor();
		endUI();
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

	void EditorUI::onEvent(SDL_Event &event)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
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

	void EditorUI::tileset()
	{
		auto data = Plutus::AssetManager::getTilesets();
		ImGui::Begin("TileSets Window");

		if (data.size() == 0)
		{
			ImGui::End();
		}
		ImDrawList *draw_list = ImGui::GetWindowDrawList();
		static int selected = 0;
		static bool mDown;

		{

			ImGui::RadioButton("Place", &m_mode, EDIT_PLACE);
			ImGui::SameLine();
			ImGui::RadioButton("Select", &m_mode, EDIT_SELECT);
			ImGui::SameLine();
			ImGui::RadioButton("Remove", &m_mode, EDIT_REMOVE);

			mTileTexture = AssetManager::getTexture(data[selected]->name);

			static int sc = 100;
			static float scale = 1.0f;
			ImGui::PushItemWidth(100);
			{
				if (ImGui::InputInt("Scale", &sc, 5))
				{
					sc = sc > 25 ? sc : 25;
					scale = sc / 100.0f;
				}
				ImGui::SameLine();
				ImGui::ComboBox("TileSheet", data, selected);
				ImGui::PopItemWidth();
			}

			ImGui::Separator();

			ImVec2 canvas_pos = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!

			ImVec2 cv_destStart(canvas_pos.x, canvas_pos.y);
			ImVec2 cv_destEnd(canvas_pos.x + mTileTexture.width * scale, canvas_pos.y + mTileTexture.height * scale);

			auto color = IM_COL32(255, 255, 255, 100);
			//background
			draw_list->AddRectFilledMultiColor(canvas_pos, cv_destEnd, IM_COL32(50, 50, 50, 255), IM_COL32(50, 50, 60, 255), IM_COL32(60, 60, 70, 255), IM_COL32(50, 50, 60, 255));

			ImGui::InvisibleButton("inv", ImVec2(mTileTexture.width * scale, mTileTexture.height * scale));
			draw_list->AddImage((void *)mTileTexture.id, canvas_pos, cv_destEnd);
			draw_list->AddRect(canvas_pos, cv_destEnd, color);

			float tileWidth = data[selected]->width * scale;
			float tileHeight = data[selected]->height * scale;

			float textureHeight = mTileTexture.height * scale;
			float textureWidth = mTileTexture.width * scale;

			for (float y = 0; y < textureHeight; y += tileHeight)
			{
				draw_list->AddLine(ImVec2(canvas_pos.x, canvas_pos.y + y),
								   ImVec2(cv_destEnd.x, canvas_pos.y + y), color, 1.0f);
			}

			for (float x = 0; x < textureWidth; x += tileWidth)
			{
				draw_list->AddLine(ImVec2(canvas_pos.x + x, canvas_pos.y),
								   ImVec2(canvas_pos.x + x, cv_destEnd.y), color, 1.0f);
			}
			//Rect
			static bool mDown = false;
			if (ImGui::IsItemHovered())
			{
				ImVec2 mpos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - canvas_pos.x, ImGui::GetIO().MousePos.y - canvas_pos.y);

				float x = floor(mpos_in_canvas.x / tileWidth);
				float y = floor(mpos_in_canvas.y / tileHeight);
				ImVec2 start(x * tileWidth + canvas_pos.x, y * tileHeight + canvas_pos.y);
				ImVec2 end(start.x + tileWidth, start.y + tileHeight);

				draw_list->AddRect(start, end, IM_COL32(255, 0, 0, 255));

				if (mIManager->onKeyPressed(SDL_BUTTON_LEFT))
				{
					mDown = true;
					Selectedtiles.clear();
				}

				if (!mIManager->onKeyDown(SDL_BUTTON_LEFT))
				{
					mDown = false;
				}

				if (mDown)
				{
					ImVec2 vec(x, y);

					auto found = std::find_if(Selectedtiles.begin(), Selectedtiles.end(),
											  [vec](const ImVec2 &m) -> bool { return m.x == vec.x && m.y == vec.y; });
					if (found == Selectedtiles.end())
					{
						Selectedtiles.push_back(vec);
						LOG_I("X:{0}, Y:{1}", x, y);
					}
				}
			}

			for (int i = 0; i < Selectedtiles.size(); i++)
			{
				ImVec2 start(Selectedtiles[i].x * tileWidth + cv_destStart.x, Selectedtiles[i].y * tileHeight + cv_destStart.y);
				ImVec2 end(start.x + tileWidth, start.y + tileHeight);
				draw_list->AddRectFilled(start, end, IM_COL32(0, 255, 255, 50));
			}
		}
		ImGui::End();
	}

	void EditorUI::LayerControls()
	{
		ImGui::Begin("Layer Control");
		static float scale = 1.0f;
		if (ImGui::InputFloat("ZOOM", &scale, 0.05f, 0.1f, 2))
		{
			scale = CHECKLIMIT(scale, 0.4, 5);
			m_camera->setScale(scale);
		}
		if (ImGui::Button("Go to center"))
		{
			m_camera->setPosition(glm::vec2(0, 0));
		}
		ImGui::Checkbox("Move Camera", &m_moveCamera);

		ImGui::Separator();
		ImGui::Text("Layers");
		ImGui::Separator();
		static bool showAddDialog = false;
		static glm::vec2 pos;
		if (ImGui::Button("Add"))
		{
			showAddDialog = true;

			pos = mIManager->getMouseCoords();
		}

		if (showAddDialog)
		{
			ImGui::SetNextWindowPos(ImVec2(pos.x + 300, pos.y + 170));
			ImGui::SetNextWindowSize(ImVec2(260.0f, 80.0f));
			if (LayerModal())
			{
				showAddDialog = false;
			}
		}
		ImGui::PushItemWidth(100);
		static int selected = 0;
		auto layers = m_EManager->getLayers();
		if (ImGui::ComboBox("Layers", layers, selected))
		{
			m_EManager->setLayer(layers[selected]->name);
		}
		ImGui::SameLine();
		ImGui::Checkbox("Visible", &layers[selected]->isVisible);

		ImGui::PopItemWidth();

		ImGui::End();
	}

	bool EditorUI::LayerModal()
	{
		bool shouldClose = false;
		ImGui::OpenPopup("New Layer");
		static char newlayer[128];
		if (ImGui::BeginPopupModal("New Layer", NULL))
		{
			ImGui::InputText("Name", newlayer, IM_ARRAYSIZE(newlayer));
			if (ImGui::Button("Save"))
			{
				m_EManager->addLayer(newlayer);
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

	void EditorUI::EntityEditor()
	{
		ImGui::Begin("Entity Editor");
		auto currentLayer = m_EManager->getCurrentLayer();
		ImGui::PushItemWidth(100);
		static int selectedEntity = 0;
		auto layers = m_EManager->getLayers();
		auto *CurEntity = currentLayer->entities[0];
		if (ImGui::ComboBox("Entities", currentLayer->entities, selectedEntity))
		{
			CurEntity = currentLayer->entities[selectedEntity];
		}

		ImGui::Separator();
		ImGui::Text("Components");
		ImGui::Separator();

		static int selectedComp = 0;
		std::vector<std::string> components({"Animation", "Image", "Transform", "Input"});
		if (ImGui::ComboBox("Components", components, selectedComp))
		{
			LOG_I("selected {0}", components[selectedComp]);
		}
		// if (selectedComp == 0)
		// {
		// }

		// if (selectedComp == 1)
		// {
		// }
		// if (selectedComp == 2)
		// {
		// }
		ImGui::End();
	}

	void EditorUI::beginWindow()
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

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
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
	}

	void EditorUI::endWindow()
	{
		ImGui::End();
	}

} // namespace Plutus