#pragma once

#include "pgr.h"

struct Viewport {
	int x, y;
	int width, height;

	Viewport(int w, int h) : width(w), height(h) {
		x = 0;
		y = 0;
	}

	Viewport(int x, int y, int w, int h) : width(w), height(h), x(x), y(y) {}
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

	Camera(const Viewport& viewport): viewport(viewport) {
		position = glm::vec3(0.0f, 0.0f, 5.0f);
		zoom = 1.0f;
		direction = glm::vec3(0.0f, 0.0f, -1.0f);
		worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
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

	void update(float dt) {
		updateView();

		mPVMatrix = mProjMatrix * mViewMatrix;
	}
};

class OrthoCamera : public Camera {
public:
	void updateProjection() override {
		float halfWidth = viewport.width / 2.0f * zoom;
		float halfHeight = viewport.height / 2.0f * zoom;
		mProjMatrix = glm::orthoRH(-halfWidth, halfWidth, -halfHeight, halfHeight, 0.1f, 100.0f);
	}

	OrthoCamera(const Viewport& viewport) : Camera(viewport) {
	}
};

class PerspectiveCamera : public Camera {
private:
	float mFov;
	float mNear;
	float mFar;
public:

	PerspectiveCamera(const Viewport& viewport, float pFov, float pNear, float pFar) : Camera(viewport), mFov(pFov), mNear(pNear), mFar(pFar) {
	}

	void updateProjection() override {
		mProjMatrix = glm::perspective(mFov, (float)viewport.width / viewport.height, mNear, mFar);
	}
};