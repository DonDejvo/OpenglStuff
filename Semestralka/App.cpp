#include "App.h"

App::App(const GameConfig& config) : mConfig(config)
{
}

void App::init()
{
}

void App::update(float dt)
{
}

void App::draw()
{
}

void App::destroy()
{
}

void App::addGameObject(GameObject* go)
{
	gameObjects.push_back(go);
	go->setGame(this);
	go->init();
	go->getMesh()->computeModelMatrix();
}

void App::removeGameObject(GameObject* go)
{
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), go), gameObjects.end());
	delete go;
}

GameObject* App::getObjectByName(const std::string& name)
{
	for (GameObject* go : gameObjects) {
		if (go->name == name) return go;
	}
	return nullptr;
}

std::vector<GameObject*> App::getObjectsByType(const std::string& type)
{
	std::vector<GameObject*> result;
	for (GameObject* go : gameObjects) {
		if (go->type == type) {
			result.push_back(go);
		}
	}
	return result;
}
