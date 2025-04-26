#pragma once

#include "pgr.h"
#include "Lights.h"
#include "Mesh.h"

class Lamp : public Mesh {
public:
	PointLight light;

	void init();
	void setPosition(const glm::vec3& pos);
};