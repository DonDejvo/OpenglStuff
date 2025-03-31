#include <iostream>
#include "Window.h"

int main(int argc, char** argv) {

    GameConfig config;
    config.title = "My game";
    config.winWidth = 1920;
    config.winHeight = 1080;

    Game game(config);

    Window::start(argc, argv, &game);

	return 0;
}