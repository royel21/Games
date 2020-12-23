#ifndef _APP_H
#define _APP_H

#include "Core/IMainGame.h"
#include "ECS/Scene.h"
class App : public Plutus::IMainGame
{
private:
    Plutus::Scene mScene;

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