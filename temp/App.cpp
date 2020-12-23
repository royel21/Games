#include "SDL.h"

#include "App.h"
#include "Log/Logger.h"
#include "Assets/AssetManager.h"
#include "ECS/Components/Transform2DComponent.h"
#include "ECS/Components/ImageComponent.h"
#include "ECS/Components/AnimationComponent.h"
#include "ECS/Components/InputComponent.h"
#include "sstream"

App::App(const char *windowName, int screenWidth, int screenHeight) : IMainGame(windowName, screenWidth, screenHeight)
{
}

App::~App()
{
    mEntManager->ClearData();
}

void App::onInit()
{
    mEntManager = Plutus::EntityManager::getInstance();
    mDebugUI = Plutus::EditorUI::getInstance(&m_window, &m_camera);

    m_textLayer.init(&m_window, "./assets/fonts/Zoika.ttf", 28);

    m_debugRender = Plutus::DebugRender::geInstances();
    m_debugRender->init(&m_window, &m_camera);
    m_debugRender->setGridSize(32, 32);

    mEntManager->init();

    m_camera.init(getWidth(), getHeight());
    m_camera.update();
    mEntManager->setCamera(&m_camera);

    Plutus::AssetManager::addTexture("BG0", "./assets/textures/Cocarico-zelda.png");

    auto &bg0 = mEntManager->AddEntity("bg0");
    auto origin = m_camera.convertScreenToWold(0, 0);
    bg0.addComponent<Plutus::Transform2DComponent>(origin, 1280, 768);
    bg0.addComponent<Plutus::ImageComponent>("BG0");

    mEntManager->addLayer("Layer1");
    mEntManager->setLayer("Layer1");
    Plutus::AssetManager::addTileSet("player", 12, 24, 32, "./assets/textures/link.png");
    Plutus::AssetManager::addTileSet("cave", 8, 32, 32, "./assets/textures/goblin_cave.png");

    auto &player = mEntManager->AddEntity("player");
    player.addComponent<Plutus::Transform2DComponent>(origin, 48.0f, 64.0f);

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

    m_camera.update();
    // enter full sceen with ALT + ENTER
    if (m_inputManager->onKeyDown(SDLK_LALT) && (m_inputManager->onKeyPressed(SDLK_KP_ENTER) || m_inputManager->onKeyPressed(SDLK_RETURN)))
    {
        m_window.setFulScreen();
    }
    //Scale using mouse wheel
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
    }
    //Save the mouse last coords
    if (m_inputManager->onKeyPressed(SDL_BUTTON_LEFT))
    {
        mDebugUI->setLastCoord(m_inputManager->getMouseCoords());
    }
    // move the camera
    if (mDebugUI->moveCamera() && m_inputManager->onKeyDown(SDL_BUTTON_LEFT))
    {
        //get the mouseCoord;
        auto coords = m_inputManager->getMouseCoords();
        // get mouse last coords
        auto last = mDebugUI->getLastCoords();
        // get the camera scale factor
        float scale = m_camera.getScale();
        //calculate distance move with scale
        auto result = (coords - last) / scale;
        //get the camera position
        auto camPos = m_camera.getPosition();

        glm::vec2 newPos(camPos.x - result.x, result.y + camPos.y);
        m_camera.setPosition(newPos);
        mDebugUI->setLastCoord(coords);
    }
    mEntManager->Update(dt);
}

void App::onDraw()
{
    mDebugUI->bindFB();
    mEntManager->Render();
    m_debugRender->drawGrid();

    char text[20];
    snprintf(text, 20, "%.1f FPS", getFPS());
    m_textLayer.setColor(1.0f, 1.0f, 1.0f);
    m_textLayer.drawString(text, m_screenWidth - 50, m_screenHeight - 25.0f, 1.0f);

    m_textLayer.setColor(1.0f, 0, 0, 0.8);
    m_textLayer.drawString("OpenGL 4.6", 5.0f, 5.0f, 1.0f);

    mDebugUI->unBindFB();

    mDebugUI->DrawUI();
}

void App::onResize(int width, int height)
{
    LOG_I("W:{0} H:{1}", width, height);
}

void App::onExit()
{
    m_debugRender->dispose();
    LOG_I("Exiting");
}

void App::onEvent(SDL_Event &event)
{
    mDebugUI->onEvent(event);
}
