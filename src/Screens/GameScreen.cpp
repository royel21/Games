#include "GameScreen.h"
#include "Core/Engine.h"

GameScreen::GameScreen()
{
}

GameScreen::~GameScreen()
{
}

int GameScreen::getNextScreenIndex() const
{
    return -1;
}

int GameScreen::getPrevScreentIndex() const
{
    return 0;
}

void GameScreen::build()
{
    mInputManager = Plutus::InputManager::getInstance();
}

void GameScreen::onEntry()
{
}

void GameScreen::onEvent(SDL_Event &event)
{
    if (mInputManager->onKeyPressed(SDL_BUTTON_RIGHT))
    {
        // mCurrentState = Plutus::ScreenState::CHANGE_PREV;
    }
}

void GameScreen::update(float deltaTime)
{
}

void GameScreen::draw()
{
    mEngine->setBackgoundColor(0.33, .33, .33, 1.0);
}

void GameScreen::onScreenResize(int w, int h)
{
}

void GameScreen::onExit()
{
}

void GameScreen::destroy()
{
}
