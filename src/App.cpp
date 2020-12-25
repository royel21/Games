
#include "App.h"
#include "ECS/Entity.h"
#include <cstdlib>
#include <ctime>
#include <Log/Logger.h>

#include "ECS/Components.h"
#include "ECS/SpriteComponent.h"
#include "Assets/AssetManager.h"

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
    mEditor = Plutus::EditorUI::getInstance(&m_window, &mCamera);

    mTextLayer.init(&m_window, "./assets/fonts/Zoika.ttf", 28);

    mScene.Init(&mCamera);
    auto pos = mCamera.convertScreenToWold(0, 0);

    auto amanager = Plutus::AssetManager::getInstance();
    //"player", 12, 24, 32, "./assets/textures/link.png"
    amanager->addTexture("player", "./assets/textures/link.png", 12, 24, 32);
    auto bg1 = amanager->addTexture("bg1", "./assets/textures/forest-zelda.png");

    auto ent = mScene.createEntity("bg1");
    ent.addComponent<Plutus::TransformComponent>(pos.x, pos.y, 1280, 768);
    ent.addComponent<Plutus::SpriteComponent>(bg1.texId);
}

void App::onUpdate(float dt)
{
    mScene.update();
    mCamera.update();
}

void App::onDraw()
{
    mEditor->bindFB();
    mScene.draw();
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
}
