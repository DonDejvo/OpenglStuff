#pragma once
#include "Camera.h"

class CameraMovement {
private:
	Camera* mCamera;
public:
	bool enabled;
	float speed;
	float yaw, pitch;

	CameraMovement();

	void update(float dt);

	inline void setCamera(Camera* camera) {
		mCamera = camera;
	}
};