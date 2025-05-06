#include "Player.h"
#include "Game.h"

Player::Player(Camera* camera)
{
	name = "player";
	type = "player";
	mPlayerMovement = new PlayerMovement(this);
	mass = 1.0f;
}

Player::~Player()
{
	delete mPlayerMovement;
}

void Player::init()
{
	mMesh->loadFromFile("data/hero/character.obj");
	mMesh->scale *= 0.5f;
}

void Player::update(float dt)
{
	GameObject::update(dt);

	mPlayerMovement->update(dt);

	for (GameObject* go : game->gameObjects) {
		if (this == go) continue;

		collide(go);
	}

	glm::vec3& pos = mMesh->position;

	if (pos.x < 0) {
		pos.x = 0;
	}
	else if (pos.x > WORLD_SIZE) {
		pos.x = WORLD_SIZE;
	}
	if (pos.z < 0) {
		pos.z = 0;
	}
	else if (pos.z > WORLD_SIZE) {
		pos.z = WORLD_SIZE;
	}

	pos.y = ((Game*)game)->terrainManager->getHeightAtPosition(pos);
}

void Player::enableMovement(bool value)
{
	mPlayerMovement->enabled = value;
}
