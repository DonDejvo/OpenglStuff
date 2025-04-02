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
	virtual void draw() const;
	virtual void destroy();
	virtual void onKey(unsigned char keyPressed);

	inline const GameConfig& getConfig() const {
		return mConfig;
	}
};