#pragma once
#include "Core/IGameScreen.h"
#include "TextRender/TextLayer.h"
#include "Graphics/Camera2D.h"
#include "ECS/EntityManager.h"
#include "Input/InputManager.h"
#include "EditorUI.h"

class EditorScreen : public Plutus::IGameScreen
{

public:
    EditorScreen();
    ~EditorScreen();

    int getNextScreenIndex() const override;
    int getPrevScreentIndex() const override;

    // called at beginning and end of the application
    void build() override;
    void destroy() override;
    // Called when a screen enter and exit focus
    void onEntry() override;
    void onExit() override;
    // Called in the main game loop
    void update(float deltaTime) override;
    void draw() override;
    void onScreenResize(int w, int h) override;
    void onEvent(SDL_Event &event) override;

private:
    Plutus::EntityManager mEntManager;
    Plutus::TextLayer mTextLayer;
    Plutus::EditorUI *mEditor = nullptr;
    Plutus::Camera2D mCamera;
    Plutus::InputManager *mInputManager = nullptr;
};