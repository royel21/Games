
#include "App.h"
#include "ECS/Entity.h"
#include "ECS/Components.h"
#include "ECS/SpriteComponent.h"

App::App(const char *windowName, int screenWidth, int screenHeight) : IMainGame(windowName, screenWidth, screenHeight)
{
}

App::~App()
{
}

void App::onInit()
{
    setBackgoundColor(0.33f, 0.33f, 0.33f, 1.0f);
    auto texture = Plutus::AssetManager::addTexture("BG0", "./assets/textures/Cocarico-zelda.png");

    mScene.Init(m_window.getScreenWidth(), m_window.getScreenHeight());
    auto ent = mScene.createEntity("Test");
    ent.addComponent<Plutus::TransformComponent>(0.0f, 0.0f, m_window.getScreenWidth(), m_window.getScreenHeight());
    ent.addComponent<Plutus::SpriteComponent>(texture.id);
}

void App::onUpdate(float dt)
{
    mScene.update();
}

void App::onDraw()
{
    mScene.draw();
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
