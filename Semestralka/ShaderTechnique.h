#pragma once

#include "Shader.h"
#include "GameObject.h"
#include "Game.h"

class Game;

class ShaderTechnique : public DrawCallbacks {
protected:
	GLuint PVMLocation;
	GLuint modelLocation;
	Shader* mShader;

public:
	ShaderTechnique(Shader* shader);
	void use() const;
	virtual void init() = 0;
	virtual void prepare(const Game& game) = 0;
	virtual void processGameObject(const GameObject& go) = 0;
};