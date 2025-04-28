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
#include "ParticleSystem.h"

constexpr float WORLD_SIZE = 600.0f;
constexpr float WORLD_MAX_HEIGHT = 30.0f;
constexpr float WORLD_OFFSET_Y = 0;

enum class DayTime {
	DAY, DUSK, NIGHT
};

const std::vector<unsigned int> flashburstIndces = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
};

const std::vector<unsigned int> torchfireIndces = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};

class Demo : public Game {
public:
	using Game::Game;

	bool menuOpened = false;
	DayTime dayTime = DayTime::DAY;

	PerspectiveCamera mainCamera;
	OrthoCamera lightCamera;
	Shader lightShader, shadowShader, simpleShader, skyboxShader, terrainShader, waterShader, canvasShader, particleShader;
	LightingTechnique lightingTechnique;
	ShadowMapTechnique shadowMapTechnique;
	SkyboxTechnique cubemapTechnique;
	SimpleTechnique simpleTechnique;
	TerrainTechnique terrainTechnique;
	WaterTechnique waterTechnique;
	ParticleTechnique particleTechnique;

	Canvas canvas;
	UIElements uiElements;

	std::vector<WaterTile> waterTiles;

	SkyBox skyboxDay, skyboxDusk, skyboxNight;
	Fog fog;

	DirectionalLight dirLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	float dirLightAngle = 0.0f;

	QuadGeometry quadGeometry;
	PlaneGeometry planeGeometry;
	CubeGeometry cubeGeometry, skyboxGeometry;
	TerrainMaterial terrainMaterial, oceanMaterial;
	Material colorMaterial;
	WaterMaterrial waterMatrial;

	std::vector<Mesh> trees;
	Mesh plane;
	Mesh cube;
	std::vector<Terrain> terrains;
	Player player;
	PlayerCamera playerCamera;

	SpriteBatch batch, grassBatch;
	Material grassMaterial;
	std::vector<Grass> grassTiles;

	std::vector<Cube> cubes;
	std::vector<Lamp> lamps;

	Spline spline;
	float t = 0.0f;

	float particlesCounter = 0.0f;
	std::vector<Particle> particles;
	Spritesheet particleSpritesheet;

	void init() override;
	void update(float dt) override;
	void draw() override;
private:
	void drawScene(const glm::vec4& clipPlane, bool tansparent);
	void drawTransparent(const glm::vec4& clipPlane);
	float getTerrainHeight(const glm::vec3& pos);
	void bindSkybox();
	void setDayTime(DayTime dayTime);
};