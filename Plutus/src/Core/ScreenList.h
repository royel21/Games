#pragma once
#include <vector>

namespace Plutus
{
    class Engine;
    class IGameScreen;

    class ScreenList
    {
    protected:
        int mCurrenScreenIndex = -1;
        Engine *mEngine = nullptr;
        std::vector<IGameScreen *> mScreens;

    public:
        ScreenList(Engine *enine);
        ~ScreenList();

        IGameScreen *moveNext();
        IGameScreen *movePrev();
        IGameScreen *getCurrent();

        void setScreent(int nextScreen);
        void addScreen(IGameScreen *newScreen);
        void destroy();
    };
} // namespace Plutus