#ifndef _APP_H
#define _APP_H

#include "IMainGame.h"
#include "Texture/TextureManager.h"
#include "TextRender/TextLayer.h"
#include "ECS/EntityManager.h"
#include "Graphics/Camera2D.h"
#include "Graphics/DebugRenderer.h"

class App : public Plutus::IMainGame
{
private:
    int count = 0;
    Plutus::TextLayer m_textLayer;
    Plutus::EntityManager m_EntityManager;
    Plutus::Camera2D m_camera;
    Plutus::DebugRender m_debugRender;

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