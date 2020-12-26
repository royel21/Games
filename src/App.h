#ifndef _APP_H
#define _APP_H

#include "TextRender/TextLayer.h"
#include "Core/IMainGame.h"
#include "Graphics/Camera2D.h"
#include "ECS/EntityManager.h"
#include "EditorUI.h"

class App : public Plutus::IMainGame
{
private:
    Plutus::EntityManager *mEntManager = nullptr;
    Plutus::TextLayer mTextLayer;
    Plutus::EditorUI *mEditor = nullptr;
    Plutus::Camera2D mCamera;

public:
    App(const char *windowName = "Default", int screenWidth = 1280, int screenHeight = 768);
    ~App();

    void onDraw() override;
    void onUpdate(float delta) override;
    void onResize(int width, int height) override;
    void onInit() override;
    void onExit() override;
    void onEvent(SDL_Event &event) override;
};
#endif