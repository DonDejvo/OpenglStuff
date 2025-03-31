#include "Lights.h"

BaseLight::BaseLight()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 0.0f;
	diffuseIntensity = 0.0f;
}

DirectionalLight::DirectionalLight()
{
	worldDirection = glm::vec3(1.0f, 0.0f, 0.0f);
}

PointLight::PointLight()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
}
