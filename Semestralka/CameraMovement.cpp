#include "CameraMovement.h"
#include "Window.h"
#include "Input.h"
#include "mathUtils.h"

CameraMovement::CameraMovement()
{
	mCamera = nullptr;
	enabled = true;
	yaw = 0;
	pitch = 0;
	speed = 5.0f;
}

void CameraMovement::update(float dt)
{
	if (!enabled) {
		return;
	}

	mCamera->direction = glm::vec3(sin(yaw) * cos(pitch), sin(pitch), -cos(yaw) * cos(pitch));

	glm::vec3 moveVec = glm::vec3(0.0f);

	if (Input::get()->isKeyDown('w')) {
		moveVec += mCamera->direction;
	}
	else if (Input::get()->isKeyDown('s')) {
		moveVec -= mCamera->direction;
	}

	if (Input::get()->isKeyDown('a')) {
		moveVec += glm::vec3(mCamera->direction.z, 0.0f, -mCamera->direction.x);
	}
	else if (Input::get()->isKeyDown('d')) {
		moveVec -= glm::vec3(mCamera->direction.z, 0.0f, -mCamera->direction.x);
	}

	if (glm::length(moveVec) > 0) {
		moveVec = glm::normalize(moveVec);
	}

	moveVec *= speed;

	mCamera->position += moveVec * dt;
}
