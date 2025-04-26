#pragma once

constexpr unsigned int INPUT_NUM_KEYS = 255;
constexpr unsigned int INPUT_NUM_MOUSE_BUTTONS = 8;

class Input {
private:
	static Input* mInstance;

	bool mKeysPressed[INPUT_NUM_KEYS]{ false };
	bool mKeysClicked[INPUT_NUM_KEYS]{ false };
	int mMouseDeltaX, mMouseDeltaY;
	int mMousePrevX, mMousePrevY;
	int mMouseX, mMouseY;
	int mWheelDelta;
	bool mMouseButtonsPressed[INPUT_NUM_MOUSE_BUTTONS]{ false };
	bool mMouseButtonsClicked[INPUT_NUM_MOUSE_BUTTONS]{ false };
public:
	static Input* get();

	Input();

	void onKeyDown(unsigned char key);
	void onKeyUp(unsigned char key);
	void onMouseDown(int button);
	void onMouseUp(int button);
	void onMouseMove(int x, int y);
	void onScroll(int direction);

	void update();

	bool isKeyDown(unsigned char key) const;
	bool isKeyClicked(unsigned char key) const;
	bool isMouseButtonDown(int button) const;
	bool isMouseButtonClicked(int button) const;
	int getMouseX() const;
	int getMouseY() const;
	int getMouseDeltaX() const;
	int getMouseDeltaY() const;
	int getWheelDelta() const;
};