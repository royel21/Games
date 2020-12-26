
#include "App.h"
#include <cstdlib>
#include <ctime>
#include <Log/Logger.h>

#include "ECS/Entity.h"
#include "Assets/AssetManager.h"

#include "ECS/Components/Transform.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/TileMap.h"

App::App(const char *windowName, int screenWidth, int screenHeight) : IMainGame(windowName, screenWidth, screenHeight)
{
}

App::~App()
{
}

void App::onInit()
{
    mCamera.init(getWidth(), getHeight());
    mCamera.update();

    mEntManager = Plutus::EntityManager::getInstance();
    mEntManager->setCamera(&mCamera);
    mEntManager->init();

    mEditor = Plutus::EditorUI::getInstance();
    mEditor->Init(&m_window, &mCamera, mEntManager);

    mTextLayer.Init(&m_window, "./assets/fonts/Zoika.ttf", 28);

    auto amanager = Plutus::AssetManager::getInstance();
    //"player", 12, 24, 32, "./assets/textures/link.png"
    amanager->addTexture("player", "./assets/textures/link.png", 12, 24, 32);
    amanager->addTexture("cave", "./assets/textures/goblin_cave.png", 8, 32, 32);

    auto bg1 = amanager->addTexture("bg1", "./assets/textures/forest-zelda.png");

    auto &ent = mEntManager->addEntity("bg1");
    ent.addComponent<Plutus::Transform>(0.0f, 0.0f, 1280, 768);
    ent.addComponent<Plutus::Sprite>("bg1");

    auto &ent2 = mEntManager->addEntity("tileMap1");
    ent2.addComponent<Plutus::TileMap>();
}

void App::onUpdate(float dt)
{
    mEntManager->update(dt);
    mCamera.update();
}

void App::onDraw()
{
    mEditor->bindFB();
    mEntManager->draw();
    char text[20];
    snprintf(text, 20, "%.1f FPS", getFPS());
    mTextLayer.setColor(1.0f, 1.0f, 1.0f);
    mTextLayer.drawString(text, m_screenWidth - 50, m_screenHeight - 25.0f, 1.0f);

    mTextLayer.setColor(1.0f, 0, 0, 0.8);
    mTextLayer.drawString("OpenGL 4.6", 5.0f, 5.0f, 1.0f);

    mEditor->unBindFB();
    mEditor->DrawUI();
}

void App::onResize(int width, int height)
{
}

void App::onExit()
{
}

void App::onEvent(SDL_Event &event)
{
    mEditor->onEvent(event);
}
