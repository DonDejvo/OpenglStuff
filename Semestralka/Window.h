#pragma once

#include <string>
#include "Game.h"

constexpr int GL_VERSION_MAJOR = 3;
constexpr int GL_VERSION_MINOR = 3;
constexpr int WIN_FRAME_RATE = 16;

class Window {
private:
	static Window* instance;

	static void onDisplay();
	static void onClose();
	static void onTimer(int);
	static void onKeyboard(unsigned char keyPressed, int mouseX, int mouseY);
	static void onReshape(int newWidth, int newHeight);

	int mWinWidth;
	int mWinHeight;
	Game* mGame;

	float mPrevTime;

	Window(Game* game, int winWidth, int winHeight);

	void update();
public:
	static void start(int argc, char* argv[], Game* game);
	static Window* get();

	inline int getWinWdth() const {
		return mWinWidth;
	}
	inline int getWinHeight() const {
		return mWinHeight;
	}
};