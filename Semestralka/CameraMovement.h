#pragma once
#include "Camera.h"

class CameraMovement {
private:
	Camera* mCamera;
public:
	bool enabled;
	float moveSpeed;

	CameraMovement();
	void setCamera(Camera* cam);
	void update(float dt);
};