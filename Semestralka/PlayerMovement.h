#pragma once
#include "Mesh.h"
#include "Terrain.h"
#include "GameObject.h"

class PlayerMovement {
private:
	GameObject* mPlayer;

public:
	bool enabled;
	float turnSpeed, moveSpeed;

	PlayerMovement();
	PlayerMovement(GameObject* player);

	void update(float dt);
	void setPlayer(GameObject* mesh);
};