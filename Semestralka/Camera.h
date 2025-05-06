#pragma once

#include "pgr.h"

struct Viewport {
	int x = 0, y = 0;
	int width = 0, height = 0;
};

class Camera {
protected:
	glm::mat4 mProjMatrix;
	glm::mat4 mViewMatrix;
	glm::mat4 mPVMatrix;
public:
	Viewport viewport;
	glm::vec3 position;
	float zoom;
	glm::vec3 direction;
	glm::vec3 worldUp;
	glm::vec3 right;
	glm::vec3 up;
	float Near;
	float Far;

	Camera() {
		position = glm::vec3(0.0f, 0.0f, 5.0f);
		zoom = 1.0f;
		direction = glm::vec3(0.0f, 0.0f, -1.0f);
		worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		Near = 1.0f;
		Far = 100.0f;
	}

	virtual void updateProjection() = 0;

	void updateView() {
		direction = glm::normalize(direction);

		glm::vec3 center = position + direction;

		right = glm::normalize(glm::cross(direction, worldUp));
		up = glm::normalize(glm::cross(right, direction));

		mViewMatrix = glm::lookAt(position, center, up);
	}

	inline glm::mat4 getProjMatrix() const {
		return mProjMatrix;
	}

	inline glm::mat4 getViewMatrix() const {
		return mViewMatrix;
	}

	inline glm::mat4 getPVMatrix() const {
		return mPVMatrix;
	}

	void onResize(int width, int height) {
		viewport.width = width;
		viewport.height = height;

		updateProjection();
	}

	void update() {
		updateView();

		mPVMatrix = mProjMatrix * mViewMatrix;
	}
};

class OrthoCamera : public Camera {
public:
	void updateProjection() override {
		float halfWidth = viewport.width / 2.0f / zoom;
		float halfHeight = viewport.height / 2.0f / zoom;
		mProjMatrix = glm::orthoRH(-halfWidth, halfWidth, -halfHeight, halfHeight, Near, Far);
	}
};

class PerspectiveCamera : public Camera {
public:
	float fov;

	PerspectiveCamera() {
		fov = glm::radians(60.0f);
	}

	void updateProjection() override {
		mProjMatrix = glm::perspective(fov, (float)viewport.width / viewport.height, Near, Far);
	}
};