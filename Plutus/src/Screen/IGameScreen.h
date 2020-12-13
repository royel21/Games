#ifndef _IGAMESCREEN_H
#define _IGAMESCREEN_H

#define SCREEN_INDEX_NO_SCREEN -1

namespace Plutus {


	class IMainGame;
	class Window;
	class Event;

	enum class ScreenState
	{
		NONE,
		RUNNING,
		EXIT_APPLICATION,
		CHANGE_NEXT,
		CHANGE_PREV
	};

	class IGameScreen
	{
	protected:
		int m_screenIndex = -1;
		ScreenState m_currentState = ScreenState::NONE;
		IMainGame* m_Game = nullptr;
		Window* m_window = nullptr;

	public:
		friend class ScreenList;

		IGameScreen() { /*Empty*/ }
		virtual ~IGameScreen() { /*Empty*/ }

		// Return the index of the next or previous screen when changing screen
		virtual int getNextScreenIndex() const = 0;
		virtual int getPrevScreentIndex() const = 0;

		// called at beginning and end of the application
		virtual void build()    =   0;

		virtual void destroy()  =   0;
		// Called when a screen enter and exit focus
		virtual void onEntry() = 0;

		virtual void onExit() = 0;
		// Called in the main game loop
		virtual void update(float deltaTime) =   0;

		virtual void draw()   =   0;

		virtual void onScreenResize(int w, int h) = 0;

		virtual void onEvent(Event& event) = 0;
		
		// Return the current screen index
		int getScreenIndex() const { return m_screenIndex; }
		// Set the parent of the screen
		void setParentGame(IMainGame* game) { m_Game = game; }

		void setRunning() { m_currentState = ScreenState::RUNNING; }

		ScreenState getState() const { return m_currentState; }
	};

}

#endif //En header declaration

