#ifndef _IMAINGAME_H
#define _IMAINGAME_H

#include <memory>
#include "Window.h"
#include "Input/InputManager.h"
#include "Input/GamePad.h"
#include "Sound/SoundManager.h"
#include "Graphics/Shader.h"

namespace Plutus
{
    class IMainGame
    {
    protected:
        float m_fps = 0.0f;
        bool m_isRunning = false;
        // return the window object
        Window m_window;
        ///Handler all the input of the window mouse, keyboard or gamepad
        InputManager *m_inputManager = nullptr;
        GamePad *m_GamePad = nullptr;

        int m_screenWidth = 1280;

        int m_screenHeight = 768;
        const char *m_winName;

    public:
        IMainGame(const char *winName = "Default", int screenWidth = 1280, int screenHeight = 768);
        virtual ~IMainGame();
        // Run and initialize the game logic
        void run();
        //Called on each frame
        virtual void onDraw() = 0;
        virtual void onResize(int width, int height) = 0;
        //Called on each frame
        virtual void onUpdate(float delta) = 0;
        //Called on a the start of the window creation use for initialization
        virtual void onInit() = 0;
        //Called on window exit used for release all allocate resource
        virtual void onExit() = 0;
        //Called on each frame
        virtual void onEvent(SDL_Event &event) = 0;

        const float getFPS() const
        {
            return m_fps;
        }

        int getHeight() { return m_window.getScreenHeight(); }
        int getWidth() { return m_window.getScreenWidth(); }

        //Set backgound color
        void setBackgoundColor(float r, float g, float b, float a)
        {
            glClearColor(r, g, b, a);
        }

    private:
        bool init();
        void sysEvents(SDL_Event &event);
        void checkEvent();
    };
} // namespace Plutus

#endif