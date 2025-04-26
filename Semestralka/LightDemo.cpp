#include "LightDemo.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include "AssetManager.h"
#include "Input.h"
#include "Cube.h"

void LightDemo::init()
{

	waterShader.loadFromFiles("shaders/water.vert", "shaders/water.frag");
	lightShader.loadFromFiles("shaders/lighting.vert", "shaders/lighting.frag");
	shadowShader.loadFromFiles("shaders/shadow.vert", "shaders/shadow.frag");
	simpleShader.loadFromFiles("shaders/simple.vert", "shaders/simple.frag");
	skyboxShader.loadFromFiles("shaders/skybox.vert", "shaders/skybox.frag");
	terrainShader.loadFromFiles("shaders/terrain.vert", "shaders/terrain.frag");
	canvasShader.loadFromFiles("shaders/canvas.vert", "shaders/canvas.frag");

	skybox.loadFromFiles({
		"data/skybox/vz_dusk_right.png",
		"data/skybox/vz_dusk_left.png",
		"data/skybox/vz_dusk_up.png",
		"data/skybox/vz_dusk_down.png",
		"data/skybox/vz_dusk_front.png",
		"data/skybox/vz_dusk_back.png"
	});

	waterTechnique.setShader(&waterShader);
	waterTechnique.init();

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

	canvas.bitmapFont = BitmapFont();
	canvas.bitmapFont.charset = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	canvas.bitmapFont.spritesheet = Spritesheet();
	canvas.bitmapFont.spritesheet.numSprites = 95;
	canvas.bitmapFont.spritesheet.cols = 16;
	canvas.bitmapFont.spritesheet.spriteWidth = 12;
	canvas.bitmapFont.spritesheet.spriteHeight = 16;
	canvas.bitmapFont.spritesheet.tex = AssetManager::get()->getTexture("data/common/pixfont-bold.png");
	canvas.shader = &canvasShader;
	canvas.defaultTexture = AssetManager::get()->getTexture("data/common/white_square.png");
	canvas.init();

	uiElements.setCanvas(&canvas);

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
	fog.Density = 0.005f;

	dirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
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
	colorMaterial.alpha = 0.5f;

	terrainMaterial.ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	terrainMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	terrainMaterial.blendMap = AssetManager::get()->getTexture("data/blendmaps/blendmap_01.png");
	terrainMaterial.heightMap = AssetManager::get()->getTexture("data/heightmaps/Heightmap.png");
	terrainMaterial.diffuseTexture = AssetManager::get()->getTexture("data/terrain/grass_green_d.jpg");
	terrainMaterial.normalMap = AssetManager::get()->getTexture("data/terrain/grass_green_n.jpg");
	terrainMaterial.diffuseTextureRed = AssetManager::get()->getTexture("data/terrain/desert_sand_d.jpg");
	terrainMaterial.normalMapRed = AssetManager::get()->getTexture("data/terrain/desert_sand_n.jpg");
	terrainMaterial.diffuseTextureGreen = AssetManager::get()->getTexture("data/terrain/mntn_brown_d.jpg");
	terrainMaterial.normalMapGreen = AssetManager::get()->getTexture("data/terrain/mntn_brown_n.jpg");
	terrainMaterial.diffuseTextureBlue = AssetManager::get()->getTexture("data/terrain/snow1_d.jpg");
	terrainMaterial.normalMapBlue = AssetManager::get()->getTexture("data/terrain/snow1_n.jpg");

	player.init();
	player.setTerrain(&terrain);

	playerCamera.setCamera(&mainCamera);
	playerCamera.setPlayer(&player);

	player.position.x = WORLD_SIZE * 0.5f;
	player.position.z = WORLD_SIZE * 0.5f + 50.0f;

	plane.setGeometry(&planeGeometry);
	plane.setMaterial(0, &terrainMaterial);
	plane.scale *= 60.0f;

	terrain.setSegments(256, 256);
	terrain.size = WORLD_SIZE;
	terrain.init();
	Terrain::HeightMapConfig heightmapConfig;
	heightmapConfig.maxHeight = WORLD_MAX_HEIGHT;
	terrain.loadFromHeightMap("data/heightmaps/Heightmap.png", heightmapConfig);
	terrain.setMaterial(&terrainMaterial);

	for (unsigned int i = 0; i < 250; ++i) {
		Mesh tree;

		unsigned int type = rand() % 3;
		float scale = rand() % 100 / 100.0f;

		switch (type) {
		case 0:
			tree.loadFromFile("data/trees/laubbaum.obj");
			tree.scale *= scale + 2.0f;
			break;
		case 1:
			tree.loadFromFile("data/fern/fern1.obj");
			tree.scale *= scale * 0.5f + 1.0f;
			break;
		case 2:
			tree.loadFromFile("data/rock/roca.obj");
			tree.scale *= scale + 0.5f;
			break;
		}

		tree.position.x = rand() % 100 / 100.0f * WORLD_SIZE;
		tree.position.z = rand() % 100 / 100.0f * WORLD_SIZE;
		tree.position.y = terrain.getHeightAtPosition(tree.position);

		trees.push_back(tree);
	}

	spline.points = {
		{WORLD_SIZE * 0.5f, 17.5f, WORLD_SIZE * 0.5f + 50.0f},
		{WORLD_SIZE * 0.5f + 50.0f, 17.5f, WORLD_SIZE * 0.5f},
		{WORLD_SIZE * 0.5f, 17.5f, WORLD_SIZE * 0.5f - 50.0f},
		{WORLD_SIZE * 0.5f - 50.0f, 17.5f, WORLD_SIZE * 0.5f}
	};
	spline.recalculateSegments();

	//cube.setGeometry(&cubeGeometry);
	//cube.setMaterial(0, &colorMaterial);
	cube.loadFromFile("data/horse/CABALLO_low_poly.obj");
	cube.scale *= 1.0f;
	cube.yaw = AI_MATH_PI;
	cube.position.x = WORLD_SIZE * 0.5f;
	cube.position.y = 17.5f;
	cube.position.z = WORLD_SIZE * 0.5f + 50.0f;

	WaterTile waterTile;
	waterTile.width = WORLD_SIZE;
	waterTile.height = WORLD_SIZE;

	waterTile.init();

	waterTile.position.x = WORLD_SIZE * 0.5f;
	waterTile.position.y = WORLD_MAX_HEIGHT * 0.15f;
	waterTile.position.z = WORLD_SIZE * 0.5f;

	waterMatrial.distortionIntensity = 0.004f;
	waterMatrial.distortionTexture = AssetManager::get()->getTexture("data/water/waterdudv.jpg");

	waterTile.setMaterial(&waterMatrial);

	waterTiles.push_back(waterTile);

	batch.init();

	for (unsigned int i = 0; i < 100; ++i) {
		Cube cube1;
		cube1.position.x = rand() % 100 / 100.0f * WORLD_SIZE;
		cube1.position.z = rand() % 100 / 100.0f * WORLD_SIZE;
		cube1.position.y = terrain.getHeightAtPosition(cube1.position) + 2.5f;
		cube1.scale *= 4;
		cube1.material = &colorMaterial;
		cubes.push_back(cube1);
		cube1.add(batch);
	}

	for (unsigned int i = 0; i < 16; ++i) {
		Lamp lamp;
		lamp.init();
		lamp.position.x = rand() % 100 / 100.0f * WORLD_SIZE;
		lamp.position.z = rand() % 100 / 100.0f * WORLD_SIZE;
		lamp.position.y = terrain.getHeightAtPosition(lamp.position);
		lamp.setPosition(lamp.position);
		lamps.push_back(lamp);
	}

	grassMaterial.ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	grassMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	grassMaterial.diffuseTexture = AssetManager::get()->getTexture("data/grass/n_grass_atlas.png");

	grassBatch.init();

	for (unsigned int i = 0; i < 5000; ++i) {
		Grass grass;

		grass.material = &grassMaterial;
		grass.size = rand() % 100 / 50.0f + 0.5f;
		grass.position.x = rand() % 100 / 100.0f * WORLD_SIZE;
		grass.position.z = rand() % 100 / 100.0f * WORLD_SIZE;
		grass.position.y = terrain.getHeightAtPosition(grass.position) + grass.size * 0.5f;

		int tileId = rand() % 64;
		grass.regionX = 1.0f / 8 * (float)(tileId % 8);
		grass.regionY = 1.0f / 8 * (float)(tileId / 8);
		grass.regionWidth = 1.0f / 8;
		grass.regionHeight = 1.0f / 8;

		grass.place(grassBatch);

		grassTiles.push_back(grass);
	}

	grassBatch.prepare();
}

void LightDemo::update(float dt)
{
	Window* win = Window::get();

	canvas.beginFrame();

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
	for (auto& lamp : lamps) {
		lamp.computeModelMatrix();
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

	t += dt * 0.25f;
	cube.position = spline.getPoint(t);
	cube.position.y = terrain.getHeightAtPosition(cube.position) + cube.scale.y * 3.0f;
	glm::vec3 p = spline.getPoint1stDerivate(t);
	cube.yaw = atan2(-p.z, p.x) + AI_MATH_PI;

	uiElements.begin(10, 10, 600, 300);
	uiElements.text("Camera", 0, 0);
	if (uiElements.button("Player", 0, 40)) {
		playerCamera.setPlayer(&player);
	}
	if (uiElements.button("Horse", 200, 40)) {
		playerCamera.setPlayer(&cube);
	}
	uiElements.end();

	canvas.endFrame();
}

void LightDemo::draw()
{
	pointLights.clear();
	for (auto& lamp : lamps) {
		pointLights.push_back(lamp.light);
	}

	batch.sort(&mainCamera);
	batch.prepare();

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
	for (auto& lamp : lamps) {
		shadowMapTechnique.draw(lamp, { &lightCamera });
	}
	shadowMapTechnique.draw(cube, { &lightCamera });
	shadowMapTechnique.draw(player, { &lightCamera });
	shadowMapTechnique.draw(terrain, { &lightCamera });

	shadowMapTechnique.unbindFBO();

	Window* win = Window::get();

	glViewport(0, 0, win->getWinWdth(), win->getWinHeight());

	drawScene(glm::vec4(0, -1, 0, 1000), false);

	/*lightingTechnique.supplyModelMatrix(plane.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * plane.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	lightPVMMatrix = lightCamera.getPVMatrix() * plane.getMatrix();
	lightingTechnique.supplyLightPVMMatrix(lightPVMMatrix);
	plane.draw((DrawCallbacks*)&lightingTechnique);*/

	for (auto& tile : waterTiles) {
		waterTechnique.bindReflectionFBO(tile, mainCamera);

		drawScene(glm::vec4(0, 1, 0, -8), true);

		waterTechnique.unbindReflectionFBO(tile, mainCamera);

		waterTechnique.bindRefractionFBO(tile, mainCamera);

		drawScene(glm::vec4(0, -1, 0, 8), true);

		waterTechnique.unbindRefractionFBO(tile, mainCamera);

		glViewport(0, 0, win->getWinWdth(), win->getWinHeight());

		waterTechnique.use();
		waterTechnique.bindTextureUnits();
		waterTechnique.supplyTime(Window::get()->getElapsedTime());

		waterMatrial.distortionTexture->bind(TEST_TEX);
		waterTechnique.supplyTest(waterMatrial.distortionIntensity);

		waterTechnique.supplyCameraPosition(mainCamera.position);
		waterTechnique.enableFog(true);
		waterTechnique.supplyFog(fog);

		waterTechnique.draw(tile, { &mainCamera });
	}

	drawTransparent(glm::vec4(0, -1, 0, 1000));

	canvas.draw();
}

void LightDemo::drawScene(const glm::vec4& clipPlane, bool transparent)
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

	glDisable(GL_CULL_FACE);
	for (auto& tree : trees) {
		lightingTechnique.draw(tree, { &mainCamera, &lightCamera });
	}
	glEnable(GL_CULL_FACE);

	for (auto& lamp : lamps) {
		lightingTechnique.draw(lamp, { &mainCamera, &lightCamera });
	}

	lightingTechnique.draw(cube, { &mainCamera, &lightCamera });
	lightingTechnique.draw(player, { &mainCamera, &lightCamera });

	glDisable(GL_CULL_FACE);

	lightingTechnique.draw(grassBatch, { &mainCamera, &lightCamera });

	glEnable(GL_CULL_FACE);

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

	if (transparent) {
		drawTransparent(clipPlane);
	}
}

void LightDemo::drawTransparent(const glm::vec4& clipPlane)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

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

	lightingTechnique.draw(batch, { &mainCamera, &lightCamera });

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}
