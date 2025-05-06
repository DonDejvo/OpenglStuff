#include "Lamp.h"
#include "Game.h"

Lamp::~Lamp()
{
	auto pointLights = ((Game*)game)->pointLights;
	pointLights.erase(std::remove(pointLights.begin(), pointLights.end(), light), pointLights.end());

	delete light;
}

void Lamp::init()
{
	mMesh->loadFromFile("data/lamp/StreetLamp.obj");
	for (Material* mat : mMesh->getMaterials()) {
		mat->bidirectionalNormals = true;
	}
	mMesh->scale *= 0.4f;
	mMesh->offset.y = 3.0f;

	light = new PointLight();
	light->color = glm::vec3(1.0f, 0.8f, 0.4f);
	light->diffuseIntensity = 1.0f;
	light->attenuation.Const = 0.32f;
	light->attenuation.Linear = 0.08f;
	light->attenuation.Exp = 0.004f;

	((Game*)game)->pointLights.push_back(light);
}

void Lamp::update(float dt)
{
	GameObject::update(dt);

	light->position = mMesh->position + glm::vec3(0, mMesh->scale.y * (12 + mMesh->offset.y), 0);
}
