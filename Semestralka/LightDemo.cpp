#include "LightDemo.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include "AssetManager.h"
#include "Input.h"

void LightDemo::init()
{
	lightShader.loadFromFiles("shaders/lighting.vert", "shaders/lighting.frag");
	shadowShader.loadFromFiles("shaders/shadow.vert", "shaders/shadow.frag");
	screenShader.loadFromFiles("shaders/screen.vert", "shaders/screen.frag");
	simpleShader.loadFromFiles("shaders/simple.vert", "shaders/simple.frag");
	skyboxShader.loadFromFiles("shaders/skybox.vert", "shaders/skybox.frag");
	terrainShader.loadFromFiles("shaders/terrain.vert", "shaders/terrain.frag");

	lightingTechnique.setShader(&lightShader);
	lightingTechnique.init();

	shadowMapTechnique.setShader(&shadowShader);
	shadowMapTechnique.init();

	screenTechnique.setShader(&screenShader);
	screenTechnique.init();

	simpleTechnique.setShader(&simpleShader);
	simpleTechnique.init();

	cubemapTechnique.setShader(&skyboxShader);
	cubemapTechnique.init();

	terrainTechnique.setShader(&terrainShader);
	terrainTechnique.init();

	mainCamera.position = glm::vec3(0.0f, 5.0f, 10.0f);
	mainCamera.direction = glm::vec3(0.0f, -0.5f, -2.0f);
	mainCamera.Near = 0.1f;
	mainCamera.Far = 1000.0f;

	cameraMovement.setCamera(&mainCamera);
	cameraMovement.enabled = false;

	lightCamera.position = glm::vec3(0.0f, 20.0f, 10.0f);
	lightCamera.direction = glm::vec3(0.0f, -2.0f, -1.0f);
	lightCamera.viewport.width = 64;
	lightCamera.viewport.height = 64;
	lightCamera.Near = 1.0f;
	lightCamera.Far = 100.0f;
	lightCamera.updateProjection();

	skybox.loadFromFiles({
		"data/skybox/vz_classic_land_right.png",
		"data/skybox/vz_classic_land_left.png",
		"data/skybox/vz_classic_land_up.png",
		"data/skybox/vz_classic_land_down.png",
		"data/skybox/vz_classic_land_front.png",
		"data/skybox/vz_classic_land_back.png"
	});

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
	terrainMaterial.diffuseTextureRed = AssetManager::get()->getTexture("data/brick_ground/Ground_01.png");
	terrainMaterial.diffuseTextureGreen = AssetManager::get()->getTexture("data/grass/Grass_04.png");
	terrainMaterial.diffuseTextureBlue = AssetManager::get()->getTexture("data/dirt/Dirt_01.png");

	player.init();
	player.setTerrain(&terrain);

	playerCamera.setCamera(&mainCamera);
	playerCamera.setPlayer(&player);

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
	cube.scale.y *= 2.0f;
	cube.position.x = 4.0f;
	cube.position.y = 2.0f;
	cube.position.z = -2.5f;

	shadowMapFBO.width = 1024;
	shadowMapFBO.height = 1024;
	shadowMapFBO.init();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightDemo::update(float dt)
{
	Window* win = Window::get();

	if (Input::get()->isKeyClicked('c')) {
		cameraMovement.enabled = !cameraMovement.enabled;
	}

	//backpack.yaw += 1.0f * dt;
	//cube.yaw += 1.0f * dt;
	//cube.pitch += -0.5f * dt;

	dirLightAngle += dt * 0.5f;
	if (dirLightAngle >= AI_MATH_TWO_PI) {
		dirLightAngle -= AI_MATH_TWO_PI;
	}

	float angle = dirLightAngle;// (dirLightAngle < AI_MATH_PI ? dirLightAngle : AI_MATH_TWO_PI - dirLightAngle) + AI_MATH_PI;

	dirLight.worldDirection.x = cos(angle) * 2.5f;
	dirLight.worldDirection.z = sin(angle) * 2.5f;

	lightCamera.position = glm::vec3(-dirLight.worldDirection.x * 5.0f + mainCamera.position.x, -dirLight.worldDirection.y * 5.0f, -dirLight.worldDirection.z * 5.0f + mainCamera.position.z);
	lightCamera.direction = glm::vec3(dirLight.worldDirection);

	for (auto& tree : trees) {
		tree.computeModelMatrix();
	}
	plane.computeModelMatrix();
	cube.computeModelMatrix();
	terrain.computeModelMatrix();
	player.update(dt);

	cameraMovement.update(dt);
	playerCamera.update(dt);

	mainCamera.onResize(win->getWinWdth(), win->getWinHeight());
	mainCamera.update(dt);

	lightCamera.update(dt);
}

void LightDemo::draw()
{
	glm::mat4 PVMMatrix, lightPVMMatrix;

	shadowMapFBO.bind();

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	shadowMapTechnique.use();

	for (auto& tree : trees) {
		lightPVMMatrix = lightCamera.getPVMatrix() * tree.getMatrix();
		shadowMapTechnique.supplyPVMMatrix(lightPVMMatrix);
		tree.draw((DrawCallbacks*)&shadowMapTechnique);
	}

	lightPVMMatrix = lightCamera.getPVMatrix() * cube.getMatrix();
	shadowMapTechnique.supplyPVMMatrix(lightPVMMatrix);
	cube.draw((DrawCallbacks*)&shadowMapTechnique);

	lightPVMMatrix = lightCamera.getPVMatrix() * player.getMatrix();
	shadowMapTechnique.supplyPVMMatrix(lightPVMMatrix);
	player.draw((DrawCallbacks*)&shadowMapTechnique);

	lightPVMMatrix = lightCamera.getPVMatrix() * terrain.getMatrix();
	shadowMapTechnique.supplyPVMMatrix(lightPVMMatrix);
	terrain.draw((DrawCallbacks*)&shadowMapTechnique);

	shadowMapFBO.unbind();

	Window* win = Window::get();

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, win->getWinWdth(), win->getWinHeight());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lightingTechnique.use();

	lightingTechnique.bindTextureUnits();

	lightingTechnique.supplyCameraPosition(mainCamera.position);
	lightingTechnique.supplyDirLight(dirLight);
	lightingTechnique.supplyPointLights(pointLights);
	lightingTechnique.supplySpotLights(spotLights);

	shadowMapFBO.getShadowMap().bind(GL_TEXTURE2);


	for (auto& tree : trees) {
		lightingTechnique.supplyModelMatrix(tree.getMatrix());
		PVMMatrix = mainCamera.getPVMatrix() * tree.getMatrix();
		lightingTechnique.supplyPVMMatrix(PVMMatrix);
		lightPVMMatrix = lightCamera.getPVMatrix() * tree.getMatrix();
		lightingTechnique.supplyLightPVMMatrix(lightPVMMatrix);
		tree.draw((DrawCallbacks*)&lightingTechnique);
	}

	lightingTechnique.supplyModelMatrix(cube.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * cube.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	lightPVMMatrix = lightCamera.getPVMatrix() * cube.getMatrix();
	lightingTechnique.supplyLightPVMMatrix(lightPVMMatrix);
	cube.draw((DrawCallbacks*)&lightingTechnique);

	lightingTechnique.supplyModelMatrix(player.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * player.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	lightPVMMatrix = lightCamera.getPVMatrix() * player.getMatrix();
	lightingTechnique.supplyLightPVMMatrix(lightPVMMatrix);
	player.draw((DrawCallbacks*)&lightingTechnique);

	terrainTechnique.use();

	terrainTechnique.bindTextureUnits();

	terrainTechnique.supplyCameraPosition(mainCamera.position);
	terrainTechnique.supplyDirLight(dirLight);
	terrainTechnique.supplyPointLights(pointLights);
	terrainTechnique.supplySpotLights(spotLights);

	shadowMapFBO.getShadowMap().bind(GL_TEXTURE2);

	terrainTechnique.supplyModelMatrix(terrain.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * terrain.getMatrix();
	terrainTechnique.supplyPVMMatrix(PVMMatrix);
	lightPVMMatrix = lightCamera.getPVMatrix() * terrain.getMatrix();
	terrainTechnique.supplyLightPVMMatrix(lightPVMMatrix);
	terrain.draw((DrawCallbacks*)&terrainTechnique);

	/*lightingTechnique.supplyModelMatrix(plane.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * plane.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	lightPVMMatrix = lightCamera.getPVMatrix() * plane.getMatrix();
	lightingTechnique.supplyLightPVMMatrix(lightPVMMatrix);
	plane.draw((DrawCallbacks*)&lightingTechnique);*/

	cubemapTechnique.use();
	skybox.bind(GL_TEXTURE0);
	cubemapTechnique.supplyPVMatrix(mainCamera.getProjMatrix() * glm::mat4(glm::mat3(mainCamera.getViewMatrix())));
	cubemapTechnique.draw();
}

void LightDemo::onMouseMove(int mouseX, int mouseY)
{
	
}
