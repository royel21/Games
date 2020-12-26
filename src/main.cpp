#define SDL_MAIN_HANDLED
#include "App.h"

#include <string>
#include <iostream>
#include <filesystem>

int main(int argc, char const *argv[])
{
    // auto tileMap = Plutus::TileMap();
    // Plutus::AssetsParser parser;
    // parser.ParserTextureFromJSON("./textures.json");
    // Plutus::AssetManager::getInstance()->Serialize(sr);
    // Plutus::Serializer sr;
    // tileMap.Serialize(sr);
    // std::cout << sr.sb.GetString() << std::endl;

    App *app = new App("Level Editor", 1280, 768);
    app->run();

    delete app;
    return 0;
}
