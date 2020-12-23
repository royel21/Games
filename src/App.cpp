
#include "App.h"
#include "ECS/Entity.h"
#include "ECS/Components.h"
#include "ECS/SpriteComponent.h"
#include <cstdlib>
#include <ctime>
#include <Log/Logger.h>

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
    // auto ent = mScene.createEntity("Test");
    // ent.addComponent<Plutus::TransformComponent>(0.0f, 0.0f, m_window.getScreenWidth(), m_window.getScreenHeight());
    // ent.addComponent<Plutus::SpriteComponent>(texture.id);
    int halfX = m_window.getScreenWidth() / 2;
    int halfY = m_window.getScreenHeight() / 2;
    std::srand(std::time(nullptr)); // use current time as seed for random generator

    for (int i = 0; i < 10000; i++)
    {
        auto ent2 = mScene.createEntity("Test2");
        float x = 1 + std::rand() / ((RAND_MAX + 1u) / 640);
        float y = 1 + std::rand() / ((RAND_MAX + 1u) / 534);
        ent2.addComponent<Plutus::TransformComponent>(x, y, 64, 64);
        ent2.addComponent<Plutus::SpriteComponent>(texture.id);

        // LOG_I("time: {0} {1}", x, y);
    }
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
