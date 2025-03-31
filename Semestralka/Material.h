#pragma once
#include "Texture.h"

class Material {
public:
	Texture* diffuseTexture;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;

	Material() {
		diffuseTexture = nullptr;

		ambientColor = glm::vec3(0.0f);
		diffuseColor = glm::vec3(0.0f);
		specularColor = glm::vec3(0.0f);
	}
};