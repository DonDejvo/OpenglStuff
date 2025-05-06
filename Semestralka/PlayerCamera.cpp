#include "PlayerCamera.h"
#include "Input.h"
#include <iostream>
#include "Window.h"

PlayerCamera::PlayerCamera()
{
	enabled = true;
	mCamera = nullptr;
	mPlayer = nullptr;

	distance = 10.0f;
	yaw = 0.0f;
	pitch = AI_MATH_HALF_PI * 0.25f;
}

PlayerCamera::PlayerCamera(GameObject* player, Camera* camera)
	: mPlayer(player), mCamera(camera)
{
	enabled = true;
	distance = 10.0f;
	yaw = 0.0f;
	pitch = AI_MATH_HALF_PI * 0.25f;
}

void PlayerCamera::update(float dt)
{
	if (!enabled || mPlayer == nullptr) return;

	Mesh* mesh = mPlayer->getMesh();

	Window* win = Window::get();
	if (Input::get()->isMouseButtonDown(GLUT_RIGHT_BUTTON)) {
		yaw -= (float)Input::get()->getMouseDeltaX() / win->getWinWdth() * 10.0f;
		pitch += (float)Input::get()->getMouseDeltaY() / win->getWinHeight() * 10.0f;
	}

	distance = std::max(distance - Input::get()->getWheelDelta() * 0.5f, 1.0f);

	glm::vec3 offset;

	float r = cos(pitch) * distance;

	offset.x = sin(mesh->yaw + yaw) * r;
	offset.y = sin(pitch) * distance;
	offset.z = cos(mesh->yaw + yaw) * r;

	glm::vec3 cameraPos = glm::vec3(mesh->position.x, mesh->position.y + 4.0f * mesh->scale.y, mesh->position.z) + offset;
	glm::vec3 cameraDir = glm::normalize(-offset);

	mCamera->position = mCamera->position + (cameraPos - mCamera->position) * std::min(5.0f * dt, 1.0f);
	mCamera->direction = mCamera->direction + (cameraDir - mCamera->direction) * std::min(5.0f * dt, 1.0f);
}
