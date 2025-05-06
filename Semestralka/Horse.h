#pragma once

#include "GameObject.h"
#include "Spline.h"

class Horse : public GameObject {
private:

	Spline* spline;
	float t = 0.0f;
public:
	glm::vec3 initialPosition;
	void init() override;
	void update(float dt);
};