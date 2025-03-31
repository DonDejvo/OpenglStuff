#pragma once

#include "Shader.h"
#include "GameObject.h"
#include <map>
#include "Camera.h"
#include "ShaderTechnique.h"

struct GameConfig {
	const char* title;
	int winWidth;
	int winHeight;
};

class Game {
private:
	GameConfig mConfig;
	std::map<std::string, Shader*> mShaders;
	ShaderTechnique* technique;
	std::map<std::string, Camera*> mCameras;
	std::vector<GameObject*> mGameObjects;
public:
	Game(const GameConfig& config);
	void init();
	void update(float dt);
	void draw() const;
	void destroy();
	void onKey(unsigned char keyPressed);

	inline const GameConfig& getConfig() const {
		return mConfig;
	}
};