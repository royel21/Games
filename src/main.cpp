#define SDL_MAIN_HANDLED
#include "App.h"

#include <string>
#include <iostream>
#include <filesystem>

#include "Assets/AssetsParser.h"
#include "Assets/AssetManager.h"
#include "Serialize/Serialize.h"

namespace fs = std::filesystem;

void printFiles(std::string path);

int main(int argc, char const *argv[])
{
    Plutus::AssetsParser parser;
    // parser.ParserTextureFromJSON("./textures.json");
    // Plutus::Serializer sr;
    // Plutus::AssetManager::getInstance()->Serialize(sr);
    // std::cout << sr.sb.GetString() << std::endl;
    // printFiles("./textures");
    App *app = new App("Level Editor", 1280, 768);
    app->run();

    delete app;
    return 0;
}

void printFiles(std::string path)
{
    for (const auto &entry : fs::directory_iterator(path))
        std::cout << entry.path() << std::endl;
}
