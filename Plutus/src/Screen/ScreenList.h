#ifndef _SCREENLIST_H  
#define _SCREENLIST_H  
#include <vector>

namespace Plutus
{
    class IGameScreen;
    class IMainGame;

    class ScreenList
    {
    protected:
        int m_currenScreenIndex = -1;
        IMainGame* m_game = nullptr;
        std::vector<IGameScreen*> m_screens;
    public:
        ScreenList(IMainGame* game);
        ~ScreenList();

        IGameScreen* moveNext();
        IGameScreen* movePrev();
        void setScreent(int nextScreen);
        void addScreen(IGameScreen* newScreen);

        void destroy();

        IGameScreen* getCurrent();
    };
}

#endif