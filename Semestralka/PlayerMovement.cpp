#include "PlayerMovement.h"
#include "Input.h"

PlayerMovement::PlayerMovement()
{
	enabled = true;
	mPlayer = nullptr;
	moveSpeed = 20.0f;
	turnSpeed = 2.5f;
}

PlayerMovement::PlayerMovement(GameObject* player)
	: mPlayer(player)
{
	enabled = true;
	moveSpeed = 20.0f;
	turnSpeed = 2.5f;
}

void PlayerMovement::update(float dt)
{
	if (!enabled) return;

	Mesh* mesh = mPlayer->getMesh();

	float currentSpeed = 0.0f;
	if (Input::get()->isKeyDown('w')) {
		currentSpeed = moveSpeed;
	}
	else if (Input::get()->isKeyDown('s')) {
		currentSpeed = -moveSpeed;
	}
	float currentTurnSpeed = 0.0f;
	if (Input::get()->isKeyDown('a')) {
		currentTurnSpeed = turnSpeed;
	}
	else if (Input::get()->isKeyDown('d')) {
		currentTurnSpeed = -turnSpeed;
	}

	mesh->yaw += currentTurnSpeed * dt;

	mesh->position.z -= cos(-mesh->yaw) * currentSpeed * dt;
	mesh->position.x += sin(-mesh->yaw) * currentSpeed * dt;
}

void PlayerMovement::setPlayer(GameObject* mesh)
{
	mPlayer = mesh;
}
