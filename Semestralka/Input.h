#pragma once

class Input {
private:
	static Input* mInstance;

	bool mKeysPressed[255]{ false };
	bool mKeysClicked[255]{ false };
	int mMouseDeltaX, mMouseDeltaY;
	int mMousePrevX, mMousePrevY;
	int mMouseX, mMouseY;
	int mWheelDelta;
	bool mMouseButtonsPressed[8]{ false };
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
	int getMouseX() const;
	int getMouseY() const;
	int getMouseDeltaX() const;
	int getMouseDeltaY() const;
	int getWheelDelta() const;
};