#include "EditorScreen.h"

#include <cstdlib>
#include <ctime>
#include <Log/Logger.h>

#include "ECS/Entity.h"
#include "Assets/AssetManager.h"

#include "ECS/Components/Transform.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/TileMap.h"
#include "Core/Engine.h"
#include "Utils.h"

EditorScreen::EditorScreen()
{
}

EditorScreen::~EditorScreen()
{
}

int EditorScreen::getNextScreenIndex() const
{
    return 1;
}

int EditorScreen::getPrevScreentIndex() const
{
    return -1;
}

void EditorScreen::build()
{
    mInputManager = Plutus::InputManager::getInstance();
    mCamera.init(810, 592);
    mCamera.update();

    mEntManager.setCamera(&mCamera);
    mEntManager.init();

    mTextLayer.Init(mEngine->getWindow(), 810, 592, "./assets/fonts/Zoika.ttf", 28);

    // auto amanager = Plutus::AssetManager::getInstance();
    // //"player", 12, 24, 32, "./assets/textures/link.png"
    // amanager->addTexture("player", "./assets/textures/link.png", 12, 24, 32);
    // amanager->addTexture("cave", "./assets/textures/goblin_cave.png", 8, 32, 32);

    // auto bg1 = amanager->addTexture("bg1", "./assets/textures/forest-zelda.png");

    // auto ent = mEntManager.addEntity("bg1");
    // ent->addComponent<Plutus::Transform>(0.0f, 0.0f, 64.0f, 64.0f);
    // auto sprite = ent->addComponent<Plutus::Sprite>("");
    // sprite.mColor = Plutus::ColorRGBA8(255, 0, 0, 255);

    // auto ent2 = mEntManager.addEntity("tileMap1");
    // ent2->addComponent<Plutus::TileMap>();

    mEditor = Plutus::EditorUI::getInstance();
    mEditor->Init(mEngine->getWindow(), &mCamera, &mEntManager);
}

void EditorScreen::onEntry()
{
}

void EditorScreen::onEvent(SDL_Event &event)
{
    mEditor->onEvent(event);
}

void EditorScreen::update(float dt)
{
    mEntManager.update(dt);
    mCamera.update();
}

void EditorScreen::draw()
{
    mEditor->bindFB();
    char text[20];
    snprintf(text, 20, "%.1f FPS", mEngine->getFPS());
    mTextLayer.setColor(1.0f, 1.0f, 1.0f);
    mTextLayer.drawString(text, 5.0f, 5.0f, 1.0f, glm::vec4(1, 0, 0, 1));

    mTextLayer.setColor(1.0f, 0, 0, 0.8);

    mEditor->unBindFB();
    mEditor->DrawUI();
}

void EditorScreen::onScreenResize(int w, int h)
{
}

void EditorScreen::onExit()
{
    delete mEditor;
}

void EditorScreen::destroy()
{
}