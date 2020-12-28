#include "ScreenList.h"
#include "IGameScreen.h"

namespace Plutus
{
    ScreenList::ScreenList(Engine *engine) : mEngine(engine)
    {
        //Empty
    }

    ScreenList::~ScreenList()
    {
        destroy();
    }

    IGameScreen *ScreenList::moveNext()
    {
        IGameScreen *currentScreen = getCurrent();
        if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN)
        {
            mCurrenScreenIndex = currentScreen->getNextScreenIndex();
        }
        return getCurrent();
    }

    IGameScreen *ScreenList::movePrev()
    {
        IGameScreen *currentScreen = getCurrent();
        if (currentScreen->getPrevScreentIndex() != SCREEN_INDEX_NO_SCREEN)
        {
            mCurrenScreenIndex = currentScreen->getPrevScreentIndex();
        }
        return getCurrent();
    }

    void ScreenList::setScreent(int nextScreen)
    {
        mCurrenScreenIndex = nextScreen;
    }

    void ScreenList::addScreen(IGameScreen *newScreen)
    {
        newScreen->mScreenIndex = static_cast<int>(mScreens.size());
        mScreens.push_back(newScreen);
        newScreen->setEngine(mEngine);
        newScreen->build();
    }

    void ScreenList::destroy()
    {
        mScreens.resize(0);
        mCurrenScreenIndex = SCREEN_INDEX_NO_SCREEN;
    }

    IGameScreen *ScreenList::getCurrent()
    {
        if (mCurrenScreenIndex == SCREEN_INDEX_NO_SCREEN)
            return nullptr;
        return mScreens[mCurrenScreenIndex];
    }
} // namespace Plutus