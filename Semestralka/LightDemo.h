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
#include "SpriteBatch.h"
#include "Grass.h"
#include "Cube.h"
#include "Lamp.h"
#include "Canvas.h"
#include "Spline.h"
#include "UIElements.h"

constexpr float WORLD_SIZE = 600.0f;
constexpr float WORLD_MAX_HEIGHT = 30.0f;

class LightDemo : public Game {
public:
	using Game::Game;

	PerspectiveCamera mainCamera;
	OrthoCamera lightCamera;
	Shader lightShader, shadowShader, simpleShader, skyboxShader, terrainShader, waterShader, canvasShader;
	LightingTechnique lightingTechnique;
	ShadowMapTechnique shadowMapTechnique;
	SkyboxTechnique cubemapTechnique;
	SimpleTechnique simpleTechnique;
	TerrainTechnique terrainTechnique;
	WaterTechnique waterTechnique;

	Canvas canvas;
	UIElements uiElements;

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
	WaterMaterrial waterMatrial;

	std::vector<Mesh> trees;
	Mesh plane;
	Mesh cube;
	Terrain terrain;
	Player player;
	PlayerCamera playerCamera;

	SpriteBatch batch, grassBatch;
	Material grassMaterial;
	std::vector<Grass> grassTiles;

	std::vector<Cube> cubes;
	std::vector<Lamp> lamps;

	Spline spline;
	float t = 0.0f;

	void init() override;
	void update(float dt) override;
	void draw() override;
private:
	void drawScene(const glm::vec4& clipPlane, bool tansparent);
	void drawTransparent(const glm::vec4& clipPlane);
};