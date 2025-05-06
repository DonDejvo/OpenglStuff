#pragma once
#include "Mesh.h"
#include "App.h"

class App;

class GameObject {
protected:
	App* game;
	Mesh* mMesh;
public:
	std::string name;
	std::string type;
	float radius = 1.0f;
	float mass = 0.0f;
	bool collidable = true;

	GameObject();
	virtual ~GameObject();
	GameObject(const std::string& name);

	inline Mesh* getMesh() {
		return mMesh;
	}

	virtual void init();
	virtual void update(float dt);

	inline void setGame(App* game) {
		this->game = game;
	}

	void collide(GameObject* other);
	float getInverseMass();

	static unsigned int ids;
	static std::string generateName();
};