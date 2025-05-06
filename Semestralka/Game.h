#pragma once

#include "Game.h"
#include "App.h"
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
#include "Lamp.h"
#include "Canvas.h"
#include "Spline.h"
#include "UIElements.h"
#include "ParticleSystem.h"
#include "LineRenderer.h"
#include "PlayerMovement.h"
#include "PickingTechnique.h"
#include "CameraMovement.h"
#include "GameObject.h"
#include "Editor.h"
#include "CubemapShadowMapTechnique.h"

constexpr float WORLD_SIZE = 1500;
constexpr float WORLD_MAX_HEIGHT = 25.0f;

enum class DayTime {
	DAY, DUSK, NIGHT
};

class Game : public App {
public:
	using App::App;

	bool menuOpened = false;
	DayTime dayTime = DayTime::DAY;

	PerspectiveCamera mainCamera, pointLightCamera;
	OrthoCamera lightCamera;
	Shader lightShader, shadowShader, simpleShader, skyboxShader, terrainShader, waterShader, canvasShader, particleShader, lineShader, pickingShader, shadowPointLightShader;
	LightingTechnique lightingTechnique;
	ShadowMapTechnique shadowMapTechnique;
	SkyboxTechnique cubemapTechnique;
	SimpleTechnique simpleTechnique;
	TerrainTechnique terrainTechnique;
	WaterTechnique waterTechnique;
	ParticleTechnique particleTechnique;
	PickingTechnique pickingTechnique;
	CubemapShadowMapTechnique cubemapShadowMapTechnique;

	Canvas canvas;
	UIElements uiElements;
	LineRenderer lineRenderer;

	Editor* editor;
	PlayerCamera* playerCamera;

	std::vector<WaterTile> waterTiles;

	SkyBox skyboxDay, skyboxDusk, skyboxNight;
	Fog fog;

	DirectionalLight dirLight;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;
	float dirLightAngle = 0.0f;

	QuadGeometry quadGeometry;
	PlaneGeometry planeGeometry;
	CubeGeometry cubeGeometry, skyboxGeometry;
	TerrainMaterial terrainMaterial, oceanMaterial;
	Material colorMaterial;
	WaterMaterrial waterMatrial;

	TerrainManager* terrainManager;

	SpriteBatch grassBatch;
	Material grassMaterial;
	std::vector<Grass> grassTiles;

	Spline* spline;
	Spline* threeTowersSpline;
	float t = 0.0f;
	bool animating = false;

	std::vector<Particle> particles;
	Spritesheet* particleSpritesheet;
	Spritesheet* grassSpritesheet;
	ParticleAnim* particleAnim;

	bool isPlaying = true;

	void init();
	void update(float dt);
	void draw();

	void addParticle(Particle p);
private:
	void shadowPass();
	void drawScene(const glm::vec4& clipPlane, bool tansparent);
	void drawTransparent(const glm::vec4& clipPlane);
	void bindSkybox();
	void setDayTime(DayTime dayTime);
	void pickingPhase();
	static std::vector<glm::vec3> generateFigureEightPoints(int count, float size);
	void setIsPlaying(bool value);
};