#pragma once
#include "Mesh.h"
#include "Camera.h"

class PlayerCamera {
private:
	Mesh* mPlayer;
	Camera* mCamera;
public:
	float distance;
	float yaw;
	float pitch;

	PlayerCamera();

	void update(float dt);
	
	inline void setCamera(Camera* camera) {
		mCamera = camera;
	}

	inline void setPlayer(Mesh* player) {
		mPlayer = player;
	}
};