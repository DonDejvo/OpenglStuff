#pragma once

#include "GameObject.h"
#include "PlayerMovement.h"
#include "PlayerCamera.h"

class Player : public GameObject {
private:
	PlayerMovement* mPlayerMovement;
	
public:
	Player(Camera* camera);
	~Player();

	void init() override;

	void update(float dt) override;

	inline PlayerMovement* getPlayerMovement() const {
		return mPlayerMovement;
	}

	void enableMovement(bool value);
};