#pragma once

#include "Game.h"
#include "LightingTechnique.h"
#include "Geometry.h"
#include "Mesh.h"
#include "ShadowMapTechnique.h"
#include "ScreenTechnique.h"
#include "SimpleTechnique.h"
#include "SkyboxTechnique.h"
#include "SkyBox.h"
#include "TerrainTechnique.h"
#include "Terrain.h"
#include "Player.h"
#include "PlayerCamera.h"

class LightDemo : public Game {
public:
	using Game::Game;

	PerspectiveCamera mainCamera;
	OrthoCamera lightCamera;
	Shader lightShader, shadowShader, screenShader, simpleShader, skyboxShader, terrainShader;
	LightingTechnique lightingTechnique;
	ShadowMapTechnique shadowMapTechnique;
	SkyboxTechnique cubemapTechnique;
	ScreenTechnique screenTechnique;
	SimpleTechnique simpleTechnique;
	TerrainTechnique terrainTechnique;

	SkyBox skybox;
	Fog fog;

	DirectionalLight dirLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	float dirLightAngle = 0.0f;

	QuadGeometry quadGeometry;
	PlaneGeometry planeGeometry;
	CubeGeometry cubeGeometry, skyboxGeometry;
	TerrainMaterial terrainMaterial;
	Material colorMaterial;

	float worldSize;

	std::vector<Mesh> trees;
	Mesh plane;
	Mesh cube;
	Terrain terrain;
	Player player;
	PlayerCamera playerCamera;

	void init() override;
	void update(float dt) override;
	void draw() override;
	void onMouseMove(int mouseX, int mouseY) override;
};