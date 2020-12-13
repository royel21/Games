#include "ScreenList.h"
#include "IGameScreen.h"

namespace Plutus
{
    ScreenList::ScreenList(IMainGame* game) : m_game(game) 
    {
        //Empty
    } 

    ScreenList::~ScreenList()
    {
        destroy();
    }

    IGameScreen* ScreenList::moveNext()
    {
        IGameScreen* currentScreen = getCurrent();
        if(currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN)
        {
            m_currenScreenIndex = currentScreen->getNextScreenIndex();
        }
        return getCurrent();
    }
    
    IGameScreen* ScreenList::movePrev()
    {
        IGameScreen* currentScreen = getCurrent();
        if(currentScreen->getPrevScreentIndex() != SCREEN_INDEX_NO_SCREEN)
        {
            m_currenScreenIndex = currentScreen->getPrevScreentIndex();
        }
        return getCurrent();
    }
    
    void ScreenList::setScreent(int nextScreen)
    {
        m_currenScreenIndex = nextScreen;
    }
    
    void ScreenList::addScreen(IGameScreen* newScreen)
    {
		newScreen->m_screenIndex = static_cast<int>(m_screens.size());
        m_screens.push_back(newScreen);
        newScreen->build();
        newScreen->setParentGame(m_game);
    }

    void ScreenList::destroy()
    {
		m_screens.resize(0);
		m_currenScreenIndex = SCREEN_INDEX_NO_SCREEN;
    }

    IGameScreen* ScreenList::getCurrent(){
		if (m_currenScreenIndex == SCREEN_INDEX_NO_SCREEN) return nullptr;
        return m_screens[m_currenScreenIndex];
    }
}