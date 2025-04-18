#pragma once

#include "Shader.h"
#include <map>
#include "Camera.h"
#include "ShaderTechnique.h"
#include "Lights.h"

class ShaderTechnique;

struct GameConfig {
	const char* title;
	int winWidth;
	int winHeight;
};

class Game {
private:
	GameConfig mConfig;
public:
	Game(const GameConfig& config);
	virtual void init();
	virtual void update(float dt);
	virtual void draw();
	virtual void destroy();

	inline const GameConfig& getConfig() const {
		return mConfig;
	}
};