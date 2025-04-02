#pragma once
#include "Texture.h"

class Material {
public:
	Texture* diffuseTexture;
	Texture* specularTexture;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;

	float shininess;

	Material() {
		diffuseTexture = nullptr;
		specularTexture = nullptr;

		ambientColor = glm::vec3(0.0f);
		diffuseColor = glm::vec3(0.0f);
		specularColor = glm::vec3(0.0f);

		shininess = 0.5f;
	}
};