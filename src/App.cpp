#include "SDL.h"

#include "App.h"
#include "Log/Logger.h"
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
    mDebugUI = Plutus::EditorUI::getInstance(&m_window, &m_camera, &m_EntityManager);

    m_textLayer.init(&m_window, "./assets/fonts/Zoika.ttf", 28);

    m_debugRender.init(&m_window, &m_camera);
    m_debugRender.setGridSize(32, 32);

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
    Plutus::AssetManager::addTileSet("player", 12, 24, 32, "./assets/textures/link.png");
    Plutus::AssetManager::addTileSet("cave", 8, 32, 32, "./assets/textures/goblin_cave.png");

    auto &player = m_EntityManager.AddEntity("player");
    player.addComponent<Plutus::Transform2DComponent>(0.0f, 0.0f, 0.0f, 0.0f, 48.0f, 64.0f, 1.0f);

    player.addComponent<Plutus::ImageComponent>("player");

    auto &ac = player.addComponent<Plutus::AnimationComponent>("player");
    player.addComponent<Plutus::InputComponent>();

    ac.AddAnimation("standing", Plutus::Animation(13, 1, 100));
    ac.AddAnimation("run", Plutus::Animation(12, 12, 100));
    ac.AddAnimation("walk-up", Plutus::Animation(0, 11, 100));
    ac.AddAnimation("walk-down", Plutus::Animation(72, 12, 100));
    ac.PlayAnimation("standing");

    /*Debug Ui*/
    mDebugUI->setTileSheet("cave");
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

    // auto mcoords = m_debugRender.getSquareCoords(m_inputManager->getMouseCoords());
    // LOG_I("X:{0} Y:{1}", mcoords.x, mcoords.y);

    if (m_inputManager->onKeyDown(SDLK_LALT) && (m_inputManager->onKeyPressed(SDLK_KP_ENTER) || m_inputManager->onKeyPressed(SDLK_RETURN)))
    {
        m_window.setFulScreen();
    }
    if (m_inputManager->onKeyDown(SDLK_LCTRL))
    {
        float scale = m_camera.getScale();

        if (m_inputManager->getMouseWheel() > 0 && scale < 5)
        {
            m_camera.setScale(scale + 0.05);
        }
        else if (m_inputManager->getMouseWheel() < 0 && scale > 0.5)
        {
            m_camera.setScale(scale - 0.05);
        }

        if (m_inputManager->getMouseWheel() > 1 || m_inputManager->getMouseWheel() < 0)
        {
            LOG_I("Wheel: {0} {1}", m_inputManager->getMouseWheel(), scale);
        }
    }

    if (m_inputManager->onKeyPressed(SDL_BUTTON_LEFT))
    {
        mDebugUI->setLastCoord(m_inputManager->getMouseCoords());
    }

    if (mDebugUI->moveCamera() && m_inputManager->onKeyDown(SDL_BUTTON_LEFT))
    {
        auto coords = m_inputManager->getMouseCoords();
        auto result = coords - mDebugUI->getLastCoords();
        LOG_I("diff: {0} {1}", coords.x, coords.y);
    }

    m_camera.update();
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

    m_debugRender.drawGrid();
    mDebugUI->DrawUI();
}

void App::onResize(int width, int height)
{
    LOG_I("W:{0} H:{1}", width, height);
}

void App::onExit()
{
    m_debugRender.dispose();
    LOG_I("Exiting");
}

void App::onEvent(SDL_Event &event)
{
    mDebugUI->onEvent(event);
}
