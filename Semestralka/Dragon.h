#pragma once

#include "GameObject.h"

class Dragon : public GameObject {
public:
	float particleSpawnDelay = 0.1f;
	float particlesCounter = 0.0f;
	float particleSpeed = 3.0f;

	void init() override;
	void update(float dt) override;
};