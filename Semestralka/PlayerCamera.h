#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "GameObject.h"

class PlayerCamera {
private:
	GameObject* mPlayer;
	Camera* mCamera;
public:
	bool enabled;
	float distance;
	float yaw;
	float pitch;

	PlayerCamera();
	PlayerCamera(GameObject* mPlayer, Camera* mCamera);

	void update(float dt);
	
	inline void setCamera(Camera* camera) {
		mCamera = camera;
	}

	inline void setPlayer(GameObject* player) {
		mPlayer = player;
	}
};