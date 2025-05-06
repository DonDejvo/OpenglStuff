#include "CameraMovement.h"
#include "Input.h"
#include "Window.h"

CameraMovement::CameraMovement()
{
	enabled = true;
	mCamera = nullptr;
	moveSpeed = 40.0f;
}

void CameraMovement::setCamera(Camera* cam)
{
	mCamera = cam;
}

void CameraMovement::update(float dt)
{
	if (!enabled) return;

	glm::vec3 moveVec = glm::vec3(0.0f);
	glm::vec3 right = glm::vec3(mCamera->right.x, 0, mCamera->right.z);

	if (Input::get()->isKeyDown('w')) {
		moveVec += mCamera->direction;
	}
	else if (Input::get()->isKeyDown('s')) {
		moveVec -= mCamera->direction;
	}
	if (Input::get()->isKeyDown('a')) {
		moveVec -= right;
	}
	else if (Input::get()->isKeyDown('d')) {
		moveVec += right;
	}

	if (glm::length(moveVec) > 0) {
		moveVec = glm::normalize(moveVec);
	}
	moveVec *= moveSpeed;

	Window* win = Window::get();
	if (Input::get()->isMouseButtonDown(GLUT_RIGHT_BUTTON)) {
		float yawDelta = (float)Input::get()->getMouseDeltaX() / win->getWinWdth() * 10.0f;
		float pitchDelta = (float)Input::get()->getMouseDeltaY() / win->getWinHeight() * 10.0f;
		glm::mat4 rotMat = glm::mat4(1.0f);
		rotMat = glm::rotate(rotMat, yawDelta, glm::vec3(0, 1, 0));
		mCamera->direction = glm::vec3(rotMat * glm::vec4(mCamera->direction, 1.0f));
		rotMat = glm::rotate(glm::mat4(1.0f), -pitchDelta, glm::vec3(mCamera->direction.z, 0, -mCamera->direction.x));
		mCamera->direction = glm::vec3(rotMat * glm::vec4(mCamera->direction, 1.0f));
	}

	mCamera->position += moveVec * dt;
}
