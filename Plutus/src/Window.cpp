#undef APIENTRY

#include "Window.h"
#include "Log/Logger.h"

extern "C"
{
	_declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

namespace Plutus
{

	Window::Window() : m_sdl_window(nullptr),
					   m_glContext(nullptr),
					   m_screenWidth(1280),
					   m_screenHeight(768)
	{
		Logger::Init();
	}

	Window::~Window()
	{
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_sdl_window);
		// Clean up
		SDL_Quit();
	}

	int Window::createWin(const char *name, int width, int height, GLuint currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
		if (currentFlags & INVISIBLE)
		{
			flags |= SDL_WINDOW_HIDDEN;
		}

		if (currentFlags & FULLSCREEN)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		if (currentFlags & BORDELESS)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		m_screenWidth = width;
		m_screenHeight = height;

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		m_sdl_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, flags);

		//quit if error creating window
		if (m_sdl_window == nullptr)
		{
			LOG_E("SDL Window could not be created");
		}
		//Create a gl context
		m_glContext = SDL_GL_CreateContext(m_sdl_window);

		//Set up glew and check for error
		gladLoadGLLoader(SDL_GL_GetProcAddress);

		//Print the OpenGL Version
		std::printf("*** OpenGL Version %s ***\n", glGetString(GL_VERSION));

		//Set backgound color
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//Set VSYNC
		SDL_GL_SetSwapInterval(0);

		//Enable alpha blend
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, m_screenWidth, m_screenHeight);
		return 0;
	}

	void Window::swapBuffer()
	{
		//Swap our buffer and draw everything to the screen
		SDL_GL_SwapWindow(m_sdl_window);
	}

	void Window::setWindowSize(int width, int height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		SDL_SetWindowSize(m_sdl_window, width, height);
		m_screenWidth = width;
		m_screenHeight = height;
	}

	void Window::setWindowTitle(std::string title)
	{
		SDL_SetWindowTitle(m_sdl_window, title.c_str());
	}
	void Window::enableBlendMode()
	{
		glEnable(GL_CULL_FACE | GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
} // namespace Plutus