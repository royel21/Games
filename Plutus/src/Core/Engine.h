#pragma once

#include <memory>
#include "Window.h"
#include "Input/InputManager.h"
#include "Input/GamePad.h"
#include "Sound/SoundManager.h"
#include "Graphics/Shader.h"
#include "glm/glm.hpp"
#include <chrono>

namespace Plutus
{
    class ScreenList;
    class IGameScreen;

    class Engine
    {
    protected:
        float mFps = 0.0f;
        bool mIsRunning = false;
        // return the window object
        Window mWindow;
        ///Handler all the input of the window mouse, keyboard or gamepad
        InputManager *mInputManager = nullptr;
        GamePad *mGamePad = nullptr;

        int mScreenWidth = 1280;

        int mScreenHeight = 768;
        const char *mWinName;
        //Screen
        std::unique_ptr<ScreenList> mScreenList = nullptr;
        IGameScreen *mCurrentScreen = nullptr;

    public:
        Engine(const char *winName = "Default", int screenWidth = 1280, int screenHeight = 768);
        virtual ~Engine();
        // Run and initialize the game logic
        void run();

        virtual void addScreen() = 0;
        //Called on a the start of the window creation use for initialization
        virtual void onInit() = 0;
        //Called on window exit used for release all allocate resource
        virtual void onEvent(SDL_Event &event) = 0;
        virtual void onExit() = 0;

        const float getFPS() const { return mFps; }

        inline int getHeight() { return mWindow.getScreenHeight(); }
        inline int getWidth() { return mWindow.getScreenHeight(); }
        inline glm::vec2 getWindowSize() { return glm::vec2(mWindow.getScreenHeight(), mWindow.getScreenHeight()); }

        inline Window *getWindow() { return &mWindow; }

        //Set backgound color
        void setBackgoundColor(float r, float g, float b, float a)
        {
            glClearColor(r, g, b, a);
        }

    private:
        bool init();
        bool update(float dt);
        void draw();
        void sysEvents(SDL_Event &event);
        void checkEvent();
        void exitGame();
        void cleanup();

    private:
        float mLastElapsed = 0.0f;
        float mFrameTime = 1.0f;
        int mnFrameTime = 0;
    };
} // namespace Plutus
