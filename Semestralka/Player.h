#pragma once

#include "Mesh.h"
#include "Terrain.h"

class Player : public Mesh {
private:
	Terrain* mTerrain;
public:
	float moveSpeed;
	float turnSpeed;

	Player();
	void init();
	void update(float dt);

	inline void setTerrain(Terrain* terrain) {
		mTerrain = terrain;
	}
};