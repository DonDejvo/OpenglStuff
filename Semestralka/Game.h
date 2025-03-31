#pragma once

#include "Shader.h"
#include "GameObject.h"
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
	std::map<std::string, Shader*> mShaders;
	std::map<std::string, Camera*> mCameras;
	std::vector<GameObject*> mGameObjects;
	DirectionalLight mDirLight;
	std::vector<PointLight> mPointLights;
	ShaderTechnique* mTechnique;
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

	inline const DirectionalLight& getDirectionalLight() const {
		return mDirLight;
	}

	inline const std::vector<PointLight>& getPointLights() const {
		return mPointLights;
	}

	inline const Camera& getCamera(const std::string& name) const {
		return *mCameras.at(name);
	}
};