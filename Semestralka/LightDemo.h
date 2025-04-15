#pragma once

#include "Game.h"
#include "LightingTechnique.h"
#include "Geometry.h"
#include "Mesh.h"
#include "ShadowMapTechnique.h"
#include "SimpleTechnique.h"
#include "SkyboxTechnique.h"
#include "SkyBox.h"
#include "TerrainTechnique.h"
#include "Terrain.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "WaterTechnique.h"
#include "WaterTile.h"

class LightDemo : public Game {
public:
	using Game::Game;

	PerspectiveCamera mainCamera;
	OrthoCamera lightCamera;
	OrthoCamera guiCamera;
	Shader lightShader, shadowShader, simpleShader, skyboxShader, terrainShader, waterShader;
	LightingTechnique lightingTechnique;
	ShadowMapTechnique shadowMapTechnique;
	SkyboxTechnique cubemapTechnique;
	SimpleTechnique simpleTechnique;
	TerrainTechnique terrainTechnique;
	WaterTechnique waterTechnique;

	std::vector<WaterTile> waterTiles;

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

	std::vector<Mesh> guiTextures;

	void init() override;
	void update(float dt) override;
	void draw() override;
private:
	void drawScene(const glm::vec4& clipPlane);
	void drawGui();
};