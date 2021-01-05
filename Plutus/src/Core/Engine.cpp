#include <SDL.h>
#include "Engine.h"
#include "Time/Timing.h"
#include "Assets/AssetManager.h"
#include "Log/Logger.h"

#include "IGameScreen.h"
#include "ScreenList.h"

namespace Plutus
{
	Engine::Engine(const char *winName, int screenWidth, int screenHeight) : mWinName(winName), mScreenWidth(screenWidth), mScreenHeight(screenHeight)
	{
		mInputManager = InputManager::getInstance();
		mGamePad = GamePad::getInstance();
		mScreenList = std::make_unique<ScreenList>(this);
	}

	Engine::~Engine()
	{
	}

	void Engine::checkEvent()
	{
		mGamePad->Update();
		mInputManager->update();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			sysEvents(event);
		}
	}

	void Engine::exitGame()
	{
		mIsRunning = false;
	}

	void Engine::run()
	{
		if (!init())
			return;

		mIsRunning = true;
		FpsLimiter limiter;

		// limiter.setMaxFPS(60.0f);

		std::chrono::system_clock::time_point startTime, endTime;
		endTime = startTime = std::chrono::system_clock::now();

		while (mIsRunning)
		{

			endTime = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = endTime - startTime;
			startTime = endTime;

			float fElapsedTime = elapsedTime.count();
			mLastElapsed = fElapsedTime;

			// float dt = limiter.begin();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			checkEvent();
			update(mLastElapsed);

			draw();

			mWindow.swapBuffer();
			// limiter.end();
			// mFps = limiter.getFPS();
			// std::cout << "dt: " << dt << std::endl;
			mFrameTime += fElapsedTime;
			mnFrameTime++;
			if (mFrameTime >= 1.0f)
			{
				mFps = mnFrameTime;
				mFrameTime -= 1.0f;
				mnFrameTime = 0;
			}
		}

		mCurrentScreen->onExit();
		if (mScreenList)
		{
			mScreenList->destroy();
			mScreenList.reset();
		}

		onExit();
		AssetManager::getInstance()->clearData();
	}

	bool Engine::update(float dt)
	{
		if (mCurrentScreen)
		{
			switch (mCurrentScreen->getState())
			{
			case ScreenState::RUNNING:
			{
				mCurrentScreen->update(dt);
				break;
			}
			case ScreenState::CHANGE_NEXT:
			{
				mCurrentScreen->onExit();
				mCurrentScreen = mScreenList->moveNext();
				if (mCurrentScreen != nullptr)
				{
					mCurrentScreen->setRunning();
					mCurrentScreen->onEntry();
				}
				break;
			}
			case ScreenState::CHANGE_PREV:
			{
				mCurrentScreen->onExit();
				mCurrentScreen = mScreenList->movePrev();
				if (mCurrentScreen != nullptr)
				{
					mCurrentScreen->setRunning();
					mCurrentScreen->onEntry();
				}
				break;
			}
			case ScreenState::EXIT_APPLICATION:
			{
				exitGame();
				return true;
				break;
			}
			default:
				break;
			}
		}
		else
		{
			exitGame();
			return true;
		}
		return false;
	}

	void Engine::draw()
	{
		if (mCurrentScreen && mCurrentScreen->getState() == ScreenState::RUNNING)
		{
			mCurrentScreen->draw();
		}
	}

	void Engine::sysEvents(SDL_Event &event)
	{
		switch (event.type)
		{
		case SDL_QUIT:
		{
			mIsRunning = false;
			break;
		}

		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				mIsRunning = false;
			}
			else
				mInputManager->pressKey(event.key.keysym.sym);
			break;
		}
		case SDL_KEYUP:
		{
			mInputManager->releaseKey(event.key.keysym.sym);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			mInputManager->pressKey(event.button.button);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			mInputManager->releaseKey(event.button.button);
			break;
		}
		case SDL_MOUSEMOTION:
		{
			mInputManager->setMouseCoords((float)event.motion.x, (float)event.motion.y);
			mInputManager->setMouseMove(true);
			break;
		}
		case SDL_WINDOWEVENT:
		{
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				// glViewport(0, 0, event.window.data1, event.window.data2);
				mWindow.setWindowSize(event.window.data1, event.window.data2);
				mCurrentScreen->onScreenResize(event.window.data1, event.window.data2);
			}
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			mInputManager->setMouseWheel(event.wheel.y);
		}
		}

		mCurrentScreen->onEvent(event);
	}

	bool Engine::init()
	{
		;
		SDL_Init(SDL_INIT_EVERYTHING);
		//Set up double buffering so we don't get any flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		mWindow.createWin(mWinName, mScreenWidth, mScreenHeight, 0);

		onInit();
		addScreen();
		mCurrentScreen = mScreenList->getCurrent();
		mCurrentScreen->onEntry();
		mCurrentScreen->setRunning();

		return true;
	}

} // namespace Plutus
