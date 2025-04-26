#include "Lamp.h"

void Lamp::init()
{
	loadFromFile("data/lamp/StreetLamp.obj");
	for (Material* mat : mData.materials) {
		mat->bidirectionalNormals = true;
	}
	scale *= 0.5f;

	light.color = glm::vec3(1.0f, 0.7f, 0.8f);
	light.diffuseIntensity = 1.0f;
	light.attenuation.Const = 0.06f;
	light.attenuation.Linear = 0.04f;
	light.attenuation.Exp = 0.01f;
}

void Lamp::setPosition(const glm::vec3& pos)
{
	position = pos;
	light.position = pos + glm::vec3(0.0f, 6.0f, 0);
}
