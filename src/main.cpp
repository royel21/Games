#define SDL_MAIN_HANDLED
#include "App.h"

#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void printFiles(std::string path);

int main(int argc, char const *argv[])
{
    //printFiles("./textures");
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
