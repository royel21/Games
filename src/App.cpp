#include "App.h"
#include "Log/Logger.h"
#include "SDL.h"
#include "Assets/AssetManager.h"
#include "ECS/Transform2DComponent.h"
#include "ECS/ImageComponent.h"
#include "ECS/AnimationComponent.h"
#include "ECS/InputComponent.h"
#include "sstream"

App::App(const char *windowName, int screenWidth, int screenHeight) : IMainGame(windowName, screenWidth, screenHeight)
{
}

App::~App()
{
    m_EntityManager.ClearData();
}

void App::onInit()
{
    m_textLayer.init(&m_window, "./assets/fonts/Zoika.ttf", 28);
    m_EntityManager.init();

    m_camera.init(getWidth(), getHeight());
    m_camera.update();

    m_EntityManager.setCamera(&m_camera);

    Plutus::AssetManager::addTexture("BG0", "./assets/textures/Cocarico-zelda.png");

    auto &bg0 = m_EntityManager.AddEntity("bg0");

    bg0.addComponent<Plutus::Transform2DComponent>((1280 / 2) * -1, (768 / 2) * -1, 0.0f, 0.0f, 1280, 768, 1.0f);
    bg0.addComponent<Plutus::ImageComponent>("BG0");

    m_EntityManager.addLayer("Layer1");
    m_EntityManager.setLayer("Layer1");
    Plutus::AssetManager::addTileSet("link-png", 12, 24, 32, "./assets/textures/link.png");

    auto &player = m_EntityManager.AddEntity("link");
    player.addComponent<Plutus::Transform2DComponent>(0.0f, 0.0f, 0.0f, 0.0f, 48.0f, 64.0f, 1.0f);

    player.addComponent<Plutus::ImageComponent>("link-png");
    player.addComponent<Plutus::InputComponent>();

    auto &ac = player.addComponent<Plutus::AnimationComponent>("link-png");

    ac.AddAnimation("standing-link", Plutus::Animation(12, 12, 50));
    ac.PlayAnimation("standing-link", true);
}

void App::onUpdate(float dt)
{
    // if (m_inputManager->onKeyPressed(SDL_BUTTON_LEFT))
    // {
    //     LOG_I("GamePad isConnected: {0}", m_GamePad->isConnected(0));
    //     count++;
    //     LOG_I("");
    //     LOG_I("Clicked: {0}", count);
    // }
    // if (m_inputManager->onKeyDown(SDL_BUTTON_RIGHT) || m_inputManager->onKeyPressed(SDLK_a))
    // {
    //     count++;
    //     LOG_I("");
    //     LOG_I("Clicked: {0}", count);
    // }

    // if (m_GamePad->isBtnPress(0, GAMEPAD_RIGHT) || m_GamePad->isBtnPress(0, GAMEPAD_UP) || m_GamePad->isBtnPress(0, GAMEPAD_DOWN))
    // {
    //     LOG_I("A:{0} B:{1} Y:{2}", m_GamePad->isBtnDown(0, GAMEPAD_RIGHT), m_GamePad->isBtnDown(0, GAMEPAD_UP), m_GamePad->isBtnDown(0, GAMEPAD_DOWN));
    // }

    // if (m_inputManager->onKeyDown(SDLK_LALT) && m_inputManager->onKeyPressed(SDLK_KP_ENTER))
    // {
    //     m_window.setFulScreen();
    // }
    m_EntityManager.Update(dt);
}

void App::onDraw()
{
    m_EntityManager.Render();

    char text[20];
    snprintf(text, 20, "%.1f FPS", getFPS());
    m_textLayer.setColor(1.0f, 1.0f, 1.0f);
    m_textLayer.drawString(text, m_screenWidth - 50, m_screenHeight - 25.0f, 1.0f);

    m_textLayer.setColor(1.0f, 0, 0, 0.8);
    m_textLayer.drawString("OpenGL 4.6", 5.0f, 5.0f, 1.0f);
}

void App::onResize(int width, int height)
{
    LOG_I("W:{0} H:{1}", width, height);
}

void App::onExit()
{
    LOG_I("Exiting");
}

void App::onEvent(SDL_Event &event)
{
}
