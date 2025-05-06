#pragma once

#pragma once

#include <map>
#include "GameObject.h"

class GameObject;

struct GameConfig {
	const char* title;
	int winWidth;
	int winHeight;
};

class App {
private:
	GameConfig mConfig;
public:
	std::vector<GameObject*> gameObjects;

	App(const GameConfig& config);
	virtual void init();
	virtual void update(float dt);
	virtual void draw();
	virtual void destroy();

	void addGameObject(GameObject* go);
	void removeGameObject(GameObject* go);

	inline const GameConfig& getConfig() const {
		return mConfig;
	}

	GameObject* getObjectByName(const std::string& name);

	std::vector<GameObject*> getObjectsByType(const std::string& type);
};