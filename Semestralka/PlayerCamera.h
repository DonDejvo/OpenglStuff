#pragma once
#include "Player.h"
#include "Camera.h"

class PlayerCamera {
private:
	Player* mPlayer;
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

	inline void setPlayer(Player* player) {
		mPlayer = player;
	}
};