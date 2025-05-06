#include <iostream>
#include "Window.h"
#include "Game.h"

int main(int argc, char** argv) {

    GameConfig config;
    config.title = "Just a Game!";
    config.winWidth = 1920;
    config.winHeight = 1080;

    Game* game = new Game(config);

    Window::start(argc, argv, game);

    delete game;

	return 0;
}