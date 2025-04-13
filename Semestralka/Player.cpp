#include "Player.h"
#include "Input.h"

Player::Player()
{
	mTerrain = nullptr;
	moveSpeed = 20.0f;
	turnSpeed = 5.0f;
}

void Player::init()
{
	loadFromFile("data/hero/character.obj");
	for (Material* material : mData.materials) {
		material->ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	scale = glm::vec3(0.5f);
}

void Player::update(float dt)
{
	float currentSpeed = 0.0f;
	if (Input::get()->isKeyDown('w')) {
		currentSpeed = moveSpeed;
	} else if (Input::get()->isKeyDown('s')) {
		currentSpeed = -moveSpeed;
	}
	float currentTurnSpeed = 0.0f;
	if (Input::get()->isKeyDown('a')) {
		currentTurnSpeed = turnSpeed;
	}
	else if (Input::get()->isKeyDown('d')) {
		currentTurnSpeed = -turnSpeed;
	}

	yaw += currentTurnSpeed * dt;

	position.z -= cos(-yaw) * currentSpeed * dt;
	position.x += sin(-yaw) * currentSpeed * dt;

	position.y = mTerrain->getHeightAtPosition(position);

	computeModelMatrix();
}