#include "PlayerCamera.h"
#include "Input.h"
#include <iostream>
#include "Window.h"

PlayerCamera::PlayerCamera()
{
	mCamera = nullptr;
	mPlayer = nullptr;

	distance = 10.0f;
	yaw = 0.0f;
	pitch = AI_MATH_HALF_PI * 0.25f;
}

void PlayerCamera::update(float dt)
{
	Window* win = Window::get();
	if (Input::get()->isMouseButtonDown(GLUT_RIGHT_BUTTON)) {
		yaw -= (float)Input::get()->getMouseDeltaX() / win->getWinWdth() * 10.0f;
		pitch += (float)Input::get()->getMouseDeltaY() / win->getWinHeight() * 10.0f;
	}

	distance -= Input::get()->getWheelDelta() * 0.5f;

	glm::vec3 offset;

	float r = cos(pitch) * distance;

	offset.x = sin(mPlayer->yaw + yaw) * r;
	offset.y = sin(pitch) * distance;
	offset.z = cos(mPlayer->yaw + yaw) * r;

	glm::vec3 cameraPos = glm::vec3(mPlayer->position.x, mPlayer->position.y + 4.0f * mPlayer->scale.y, mPlayer->position.z) + offset;
	glm::vec3 cameraDir = glm::normalize(-offset);

	mCamera->position = mCamera->position + (cameraPos - mCamera->position) * std::min(5.0f * dt, 1.0f);
	mCamera->direction = mCamera->direction + (cameraDir - mCamera->direction) * std::min(5.0f * dt, 1.0f);
}
