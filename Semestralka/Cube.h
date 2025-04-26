#pragma once
#include "SpriteBatch.h"

class Cube {
public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	Material* material;

	void add(SpriteBatch& batch);
};