#include "LightDemo.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include "AssetManager.h"
#include "Input.h"

void LightDemo::init()
{
	worldSize = 600.0f;

	lightShader.loadFromFiles("shaders/lighting.vert", "shaders/lighting.frag");
	shadowShader.loadFromFiles("shaders/shadow.vert", "shaders/shadow.frag");
	simpleShader.loadFromFiles("shaders/simple.vert", "shaders/simple.frag");
	skyboxShader.loadFromFiles("shaders/skybox.vert", "shaders/skybox.frag");
	terrainShader.loadFromFiles("shaders/terrain.vert", "shaders/terrain.frag");
	waterShader.loadFromFiles("shaders/water.vert", "shaders/water.frag");

	skybox.loadFromFiles({
		"data/skybox/vz_clear_right.png",
		"data/skybox/vz_clear_left.png",
		"data/skybox/vz_clear_up.png",
		"data/skybox/vz_clear_down.png",
		"data/skybox/vz_clear_front.png",
		"data/skybox/vz_clear_back.png"
	});

	lightingTechnique.setShader(&lightShader);
	lightingTechnique.init();

	shadowMapTechnique.setShader(&shadowShader);
	shadowMapTechnique.init();

	simpleTechnique.setShader(&simpleShader);
	simpleTechnique.init();

	cubemapTechnique.setShader(&skyboxShader);
	cubemapTechnique.init();

	terrainTechnique.setShader(&terrainShader);
	terrainTechnique.init();

	waterTechnique.setShader(&waterShader);
	waterTechnique.init();

	mainCamera.position = glm::vec3(0.0f, 5.0f, 10.0f);
	mainCamera.direction = glm::vec3(0.0f, -0.5f, -2.0f);
	mainCamera.Near = 0.1f;
	mainCamera.Far = 1000.0f;

	lightCamera.viewport.width = 64;
	lightCamera.viewport.height = 64;
	lightCamera.Near = 0.1f;
	lightCamera.Far = 100.0f;
	lightCamera.updateProjection();

	fog.Color = glm::vec3(0.9f, 0.9f, 0.95f);
	fog.Density = 0.006f;

	dirLight.diffuseIntensity = 0.9f;
	dirLight.ambientIntensity = 0.1f;
	dirLight.worldDirection = glm::vec3(0.0f, -2.0f, -1.0f);

	/*PointLight light1;
	light1.color = glm::vec3(1.0f, 0.0f, 0.0f);
	light1.position = glm::vec3(-5.0f, 2.0f, 2.5f);
	light1.diffuseIntensity = 1.0f;
	light1.attenuation.Linear = 0.2f;
	light1.attenuation.Exp = 0.04f;
	pointLights.push_back(light1);

	SpotLight light2;
	light2.color = glm::vec3(1.0f, 1.0f, 0.5f);
	light2.position = glm::vec3(2.5f, 5.0f, 2.5f);
	light2.diffuseIntensity = 1.0f;
	light2.direction = glm::vec3(-2.5f, -2.5f, -2.5f);
	light2.cutOff = cos(glm::radians(10.0f));
	spotLights.push_back(light2);

	PointLight light3;
	light3.color = glm::vec3(0.0f, 0.0f, 1.0f);
	light3.position = glm::vec3(5.0f, 2.0f, 2.5f);
	light3.diffuseIntensity = 1.0f;
	light3.attenuation.Linear = 0.2f;
	light3.attenuation.Exp = 0.04f;
	pointLights.push_back(light3);

	SpotLight light4;
	light4.color = glm::vec3(1.0f, 1.0f, 0.5f);
	light4.position = glm::vec3(-2.5f, 5.0f, 2.5f);
	light4.diffuseIntensity = 1.0f;
	light4.direction = glm::vec3(2.5f, -2.5f, -2.5f);
	light4.cutOff = cos(glm::radians(10.0f));
	spotLights.push_back(light4);*/

	planeGeometry.init();
	planeGeometry.initBuffers();

	quadGeometry.init();
	quadGeometry.initBuffers();

	cubeGeometry.init();
	cubeGeometry.computeTangents();
	cubeGeometry.initBuffers();

	skyboxGeometry.scale *= 2.0f;
	skyboxGeometry.init();
	skyboxGeometry.initBuffers();

	cubemapTechnique.setGeomtry(&skyboxGeometry);

	colorMaterial.ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	colorMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);

	terrainMaterial.ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	terrainMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	terrainMaterial.blendMap = AssetManager::get()->getTexture("data/blendmaps/blendmap_01.png");
	terrainMaterial.diffuseTexture = AssetManager::get()->getTexture("data/grass/Grass_01.png");
	terrainMaterial.normalMap = AssetManager::get()->getTexture("data/grass/Grass_01_Nrm.png");
	terrainMaterial.diffuseTextureRed = AssetManager::get()->getTexture("data/brick_ground/Ground_01.png");
	terrainMaterial.normalMapRed = AssetManager::get()->getTexture("data/brick_ground/Ground_01_Nrm.png");
	terrainMaterial.diffuseTextureGreen = AssetManager::get()->getTexture("data/grass/Grass_04.png");
	terrainMaterial.normalMapGreen = AssetManager::get()->getTexture("data/grass/Grass_04_Nrm.png");
	terrainMaterial.diffuseTextureBlue = AssetManager::get()->getTexture("data/dirt/Dirt_01.png");
	terrainMaterial.normalMapBlue = AssetManager::get()->getTexture("data/dirt/Dirt_01_Nrm.png");

	player.init();
	player.setTerrain(&terrain);

	playerCamera.setCamera(&mainCamera);
	playerCamera.setPlayer(&player);

	player.position.x = worldSize * 0.5f;
	player.position.z = worldSize * 0.5f + 50.0f;

	plane.setGeometry(&planeGeometry);
	plane.setMaterial(0, &terrainMaterial);
	plane.scale *= 60.0f;

	terrain.setSegments(255, 255);
	terrain.size = worldSize;
	terrain.init();
	Terrain::HeightMapConfig heightmapConfig;
	heightmapConfig.maxHeight = 20.0f;
	terrain.loadFromHeightMap("data/heightmaps/single_hill.png", heightmapConfig);
	terrain.setMaterial(&terrainMaterial);

	for (unsigned int i = 0; i < 100; ++i) {
		Mesh tree;
		tree.loadFromFile("data/trees/tree.obj");
		tree.position.x = rand() % 100 / 100.0f * worldSize;
		tree.position.z = rand() % 100 / 100.0f * worldSize;
		tree.position.y = terrain.getHeightAtPosition(tree.position);

		trees.push_back(tree);
	}

	cube.setGeometry(&cubeGeometry);
	cube.setMaterial(0, &colorMaterial);
	cube.scale *= 2.0f;
	cube.position.x = worldSize * 0.5f;
	cube.position.y = 12.5f;
	cube.position.z = worldSize * 0.5f + 50.0f;

	WaterTile waterTile;
	waterTile.width = 100.0f;
	waterTile.height = 100.0f;

	waterTile.init();

	waterTile.position.x = worldSize * 0.5f + 20.0f;
	waterTile.position.y = 8.0f;
	waterTile.position.z = worldSize * 0.5f;

	waterMatrial.distortionIntensity = 0.004f;
	waterMatrial.distortionTexture = AssetManager::get()->getTexture("data/water/waterdudv.jpg");

	waterTile.setMaterial(&waterMatrial);

	waterTiles.push_back(waterTile);

	Mesh guiTex1;
	guiTex1.position.x = -500;
	guiTex1.position.y = 400;
	guiTex1.scale.x = 256;
	guiTex1.scale.y = 256;
	guiTex1.setGeometry(&quadGeometry);

	guiTextures.push_back(guiTex1);

	Mesh guiTex2;
	guiTex2.position.x = -200;
	guiTex2.position.y = 400;
	guiTex2.scale.x = 256;
	guiTex2.scale.y = 256;
	guiTex2.setGeometry(&quadGeometry);

	guiTextures.push_back(guiTex2);
}

void LightDemo::update(float dt)
{
	Window* win = Window::get();

	//backpack.yaw += 1.0f * dt;
	//cube.yaw += 1.0f * dt;
	//cube.pitch += -0.5f * dt;

	dirLightAngle += dt * 0.5f;
	if (dirLightAngle >= AI_MATH_TWO_PI) {
		dirLightAngle -= AI_MATH_TWO_PI;
	}

	float angle = dirLightAngle;// (dirLightAngle < AI_MATH_PI ? dirLightAngle : AI_MATH_TWO_PI - dirLightAngle) + AI_MATH_PI;

	angle = 3.0f;

	dirLight.worldDirection.x = cos(angle) * 1.5f;
	dirLight.worldDirection.z = sin(angle) * 1.5f;

	lightCamera.position = player.position - dirLight.worldDirection * 10.0f;
	lightCamera.direction = dirLight.worldDirection;

	for (auto& tree : trees) {
		tree.computeModelMatrix();
	}
	plane.computeModelMatrix();
	cube.computeModelMatrix();
	terrain.computeModelMatrix();
	player.update(dt);
	for (auto& tile : waterTiles) {
		tile.computeModelMatrix();
	}

	playerCamera.update(dt);

	mainCamera.onResize(win->getWinWdth(), win->getWinHeight());
	mainCamera.update();

	lightCamera.update();

	guiCamera.onResize(win->getWinWdth(), win->getWinHeight());
	guiCamera.update();

	for (auto& guiTex : guiTextures) {
		guiTex.computeModelMatrix();
	}
}

void LightDemo::draw()
{
	glm::mat4 PVMMatrix, lightPVMMatrix;

	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	shadowMapTechnique.bindFBO();

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	shadowMapTechnique.use();

	for (auto& tree : trees) {
		shadowMapTechnique.draw(tree, { &lightCamera });
	}
	shadowMapTechnique.draw(cube, { &lightCamera });
	shadowMapTechnique.draw(player, { &lightCamera });
	shadowMapTechnique.draw(terrain, { &lightCamera });

	shadowMapTechnique.unbindFBO();

	Window* win = Window::get();

	glViewport(0, 0, win->getWinWdth(), win->getWinHeight());

	drawScene(glm::vec4(0, -1, 0, 1000));

	/*lightingTechnique.supplyModelMatrix(plane.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * plane.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	lightPVMMatrix = lightCamera.getPVMatrix() * plane.getMatrix();
	lightingTechnique.supplyLightPVMMatrix(lightPVMMatrix);
	plane.draw((DrawCallbacks*)&lightingTechnique);*/

	for (auto& tile : waterTiles) {
		waterTechnique.bindReflectionFBO(tile, mainCamera);

		drawScene(glm::vec4(0, 1, 0, -8));

		waterTechnique.unbindReflectionFBO(tile, mainCamera);

		waterTechnique.bindRefractionFBO(tile, mainCamera);

		drawScene(glm::vec4(0, -1, 0, 8));

		waterTechnique.unbindRefractionFBO(tile, mainCamera);

		glViewport(0, 0, win->getWinWdth(), win->getWinHeight());

		waterTechnique.use();
		waterTechnique.bindTextureUnits();
		waterTechnique.supplyTime(Window::get()->getElapsedTime());

		waterTechnique.supplyCameraPosition(mainCamera.position);
		waterTechnique.enableFog(true);
		waterTechnique.supplyFog(fog);

		waterTechnique.draw(tile, { &mainCamera });
	}

	drawGui();
}

void LightDemo::drawScene(const glm::vec4& clipPlane)
{
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lightingTechnique.use();

	lightingTechnique.bindTextureUnits();

	lightingTechnique.supplyCameraPosition(mainCamera.position);
	lightingTechnique.supplyDirLight(dirLight);
	lightingTechnique.supplyPointLights(pointLights);
	lightingTechnique.supplySpotLights(spotLights);

	lightingTechnique.enableFog(true);
	lightingTechnique.supplyFog(fog);

	lightingTechnique.supplyClipPlane(clipPlane);

	shadowMapTechnique.bindShadowMap();

	for (auto& tree : trees) {
		lightingTechnique.draw(tree, { &mainCamera, &lightCamera });
	}
	lightingTechnique.draw(cube, { &mainCamera, &lightCamera });
	lightingTechnique.draw(player, { &mainCamera, &lightCamera });

	terrainTechnique.use();

	terrainTechnique.bindTextureUnits();

	terrainTechnique.supplyCameraPosition(mainCamera.position);
	terrainTechnique.supplyDirLight(dirLight);
	terrainTechnique.supplyPointLights(pointLights);
	terrainTechnique.supplySpotLights(spotLights);

	terrainTechnique.enableFog(true);
	terrainTechnique.supplyFog(fog);

	terrainTechnique.supplyClipPlane(clipPlane);

	shadowMapTechnique.bindShadowMap();

	terrainTechnique.draw(terrain, { &mainCamera, &lightCamera });

	glCullFace(GL_FRONT);

	cubemapTechnique.use();
	skybox.bind(DIFFUSE);
	cubemapTechnique.supplyPVMatrix(mainCamera.getProjMatrix() * glm::mat4(glm::mat3(mainCamera.getViewMatrix())));
	cubemapTechnique.draw();

	glCullFace(GL_BACK);
}

void LightDemo::drawGui()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	simpleTechnique.use();

	unsigned int i = 0;

	for (auto& guiTex : guiTextures) {
		if (i == 0) {
			auto& reflectionFramebuffer = waterTechnique.getReflectionFramebuffer();
			reflectionFramebuffer.getAttachment(0).bind(DIFFUSE);
		}
		else if (i == 1) {
			auto& refractionFramebuffer = waterTechnique.getRefractionFramebuffer();
			refractionFramebuffer.getAttachment(0).bind(DIFFUSE);
		}

		glm::mat4 PVMMatrix = guiCamera.getPVMatrix() * guiTex.getMatrix();
		simpleTechnique.supplyPVMMatrix(PVMMatrix);
		guiTex.draw(nullptr);
		++i;
	}
}
