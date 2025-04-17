#pragma once

#include "pgr.h"
#include "Material.h"
#include "SpriteBatch.h"

class Grass {
public:
	Material* material;
	glm::vec3 position;
	float size;
	float regionX, regionY, regionWidth, regionHeight;

	void place(SpriteBatch& spriteBatch);
};