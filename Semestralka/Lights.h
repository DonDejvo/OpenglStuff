#pragma once

#include "pgr.h"

class BaseLight {
public:
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;

	BaseLight();
};

class DirectionalLight : public BaseLight {
public:
	glm::vec3 worldDirection;

	DirectionalLight();
};

struct Attenuation {
	float Const = 1.0f;
	float Linear = 0.0f;
	float Exp = 0.0f;
};

class PointLight : public BaseLight {
public:
	glm::vec3 position;
	Attenuation attenuation;

	PointLight();
};