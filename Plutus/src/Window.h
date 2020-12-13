#ifndef _WINDOW_H
#define _WINDOW_H

#include "Graphics/GLheaders.h"
#include <SDL.h>
#include <string>

namespace Plutus
{

	enum WindowFlags
	{
		INVISIBLE = 0x01,
		FULLSCREEN = 0x02,
		BORDELESS = 0x04
	};

	class Window
	{
	public:
		Window();
		~Window();

		int createWin(const char *name, int width, int height, GLuint currentFlags);

		void swapBuffer();

		void setWindowSize(int width, int height);

		int getScreenHeight() { return m_screenHeight; };
		int getScreenWidth() { return m_screenWidth; };

		void setWindowTitle(std::string title);
		inline void setFulScreen()
		{
			isFullScreen = !isFullScreen;
			SDL_SetWindowFullscreen(m_sdl_window, isFullScreen);
		}
		void enableBlendMode();
		//Show or hide the window cursor
		inline void ShowMouseCursor(int show) { SDL_ShowCursor(show); }
		///Retrun the sdl_window object
		SDL_Window *getSDLWindow() { return m_sdl_window; }
		/// Return the opengl context create by sdl
		SDL_GLContext getGLContext() { return m_glContext; }

	private:
		SDL_Window *m_sdl_window;
		SDL_GLContext m_glContext;
		int m_screenWidth;
		int m_screenHeight;
		bool isFullScreen = false;
	};
} // namespace Plutus

#endif