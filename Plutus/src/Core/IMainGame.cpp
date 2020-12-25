#include <SDL.h>
#include "IMainGame.h"
#include "Time/Timing.h"
#include "Assets/AssetManager.h"
#include "Log/Logger.h"

namespace Plutus
{
	IMainGame::IMainGame(const char *winName, int screenWidth, int screenHeight) : m_winName(winName), m_screenWidth(screenWidth), m_screenHeight(screenHeight)
	{
		m_inputManager = InputManager::getInstance();
		m_GamePad = GamePad::getInstance();
	}

	IMainGame::~IMainGame()
	{
	}

	void IMainGame::checkEvent()
	{
		m_GamePad->Update();
		m_inputManager->update();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			sysEvents(event);
			onEvent(event);
		}
	}

	void IMainGame::run()
	{
		if (!init())
			return;

		m_isRunning = true;
		FpsLimiter limiter;

		limiter.setMaxFPS(60.0f);
		uint32_t lastTick = SDL_GetTicks();

		while (m_isRunning)
		{
			limiter.begin();
			auto newTick = SDL_GetTicks();
			auto elapsed = newTick - lastTick;
			float dt = 16.6667 / elapsed;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			checkEvent();
			onUpdate(dt);
			onDraw();
			m_window.swapBuffer();
			limiter.end();
			m_fps = limiter.getFPS();
			lastTick = newTick;
		}
		onExit();
		AssetManager::getInstance()->clearData();
	}

	void IMainGame::sysEvents(SDL_Event &event)
	{
		switch (event.type)
		{
		case SDL_QUIT:
		{
			m_isRunning = false;
			break;
		}

		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				m_isRunning = false;
			}
			else
				m_inputManager->pressKey(event.key.keysym.sym);
			break;
		}
		case SDL_KEYUP:
		{
			m_inputManager->releaseKey(event.key.keysym.sym);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			m_inputManager->pressKey(event.button.button);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			m_inputManager->releaseKey(event.button.button);
			break;
		}
		case SDL_MOUSEMOTION:
		{
			m_inputManager->setMouseCoords((float)event.motion.x, (float)event.motion.y);
			break;
		}
		case SDL_WINDOWEVENT:
		{
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				glViewport(0, 0, event.window.data1, event.window.data2);
				m_window.setWindowSize(event.window.data1, event.window.data2);
				onResize(event.window.data1, event.window.data2);
			}
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			m_inputManager->setMouseWheel(event.wheel.y);
		}
		}
	}

	bool IMainGame::init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		//Set up double buffering so we don't get any flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		m_window.createWin(m_winName, m_screenWidth, m_screenHeight, 0);
		onInit();
		return true;
	}

} // namespace Plutus
