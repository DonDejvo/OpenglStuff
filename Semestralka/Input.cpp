#include "Input.h"
#include <iostream>

Input* Input::mInstance = nullptr;

Input* Input::get()
{
    if (mInstance == nullptr) {
        mInstance = new Input();
    }
    return mInstance;
}

Input::Input()
{
    mMouseX = 0;
    mMouseY = 0;
    mMousePrevX = 0;
    mMousePrevY = 0;
    mMouseDeltaX = 0;
    mMouseDeltaY = 0;
    mWheelDelta = 0;
}

void Input::onKeyDown(unsigned char key)
{
    mKeysPressed[key] = true;
    mKeysClicked[key] = true;
}

void Input::onKeyUp(unsigned char key)
{
    mKeysPressed[key] = false;
}

void Input::onMouseDown(int button)
{
    if (button < 8) {
        mMouseButtonsPressed[button] = true;
    }
}

void Input::onMouseUp(int button)
{
    if (button < 8) {
        mMouseButtonsPressed[button] = false;
    }
}

void Input::onMouseMove(int x, int y)
{
    mMouseX = x;
    mMouseY = y;
}

void Input::onScroll(int direction)
{
    mWheelDelta += direction;
}

void Input::update()
{
    for (unsigned int i = 0; i < 255; ++i) {
        mKeysClicked[i] = false;
    }

    mMouseDeltaX = mMouseX - mMousePrevX;
    mMouseDeltaY = mMouseY - mMousePrevY;
    mMousePrevX = mMouseX;
    mMousePrevY = mMouseY; 
    mWheelDelta = 0;
}

bool Input::isKeyDown(unsigned char key) const
{
    return mKeysPressed[key];
}

bool Input::isKeyClicked(unsigned char key) const
{
    return mKeysClicked[key];
}

bool Input::isMouseButtonDown(int button) const
{
    return button < 8 ? mMouseButtonsPressed[button] : false;
}

int Input::getMouseX() const
{
    return mMouseX;
}

int Input::getMouseY() const
{
    return mMouseY;
}

int Input::getMouseDeltaX() const
{
    return mMouseDeltaX;
}

int Input::getMouseDeltaY() const
{
    return mMouseDeltaY;
}

int Input::getWheelDelta() const
{
    return mWheelDelta;
}


