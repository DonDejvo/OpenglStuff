#include "MageTower.h"
#include "Game.h"
#include "mathUtils.h"

MageTower::~MageTower()
{
	auto spotLights = ((Game*)game)->spotLights;
	spotLights.erase(std::remove(spotLights.begin(), spotLights.end(), light), spotLights.end());

	delete light;
}

void MageTower::init()
{
	mMesh->loadFromFile("data/magetower/mage_tower.obj");
	mMesh->scale *= 4.0f;
	for (Material* mat : mMesh->getMaterials()) {
		mat->bidirectionalNormals = true;
	}
	radius = 4.0f;

	light = new SpotLight();
	light->diffuseIntensity = 1.0f;
	light->attenuation.Const = 1.0f;
	light->cutOff = 0.98f;

	((Game*)game)->spotLights.push_back(light);
}

void MageTower::update(float dt)
{
	GameObject::update(dt);

	angle += 0.5f * dt;
	if (angle > AI_MATH_TWO_PI) {
		angle = 0.0f;
	}

	light->color = HSLtoRGB(glm::vec3(angle / AI_MATH_TWO_PI, 1.0f, 0.5f));
	light->direction = glm::vec3(sin(angle), -2.0, cos(angle));
	light->position = mMesh->position + glm::vec3(0, mMesh->scale.y * (8.0f + mMesh->offset.y), 0);
}
