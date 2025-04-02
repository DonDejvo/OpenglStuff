#include <iostream>
#include "Window.h"
#include "LightDemo.h"

int main(int argc, char** argv) {

    GameConfig config;
    config.title = "Just a Game!";
    config.winWidth = 1920;
    config.winHeight = 1080;

    LightDemo game(config);

    Window::start(argc, argv, &game);

	return 0;
}