#pragma once
#include "Texture.h"

class Material {
public:
	Texture* diffuseTexture;
	Texture* specularTexture;
	Texture* normalMap;

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;

	float shininess;
	float alpha;
	bool bidirectionalNormals;

	Material() {
		diffuseTexture = nullptr;
		specularTexture = nullptr;
		normalMap = nullptr;

		ambientColor = glm::vec3(0.0f);
		diffuseColor = glm::vec3(0.0f);
		specularColor = glm::vec3(0.0f);

		shininess = 0.5f;
		alpha = 1.0f;
		bidirectionalNormals = false;
	}
};

class TerrainMaterial : public Material {
public:
	Texture* diffuseTextureRed;
	Texture* diffuseTextureGreen;
	Texture* diffuseTextureBlue;
	Texture* normalMapRed;
	Texture* normalMapGreen;
	Texture* normalMapBlue;
};

class WaterMaterrial : public Material {
public:
	Texture* distortionTexture = nullptr;
	float distortionIntensity = 0.0f;
};