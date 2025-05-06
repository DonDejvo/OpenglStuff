#include "Game.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include "AssetManager.h"
#include "Input.h"
#include "mathUtils.h"
#include "Horse.h"

void Game::init()
{
	waterShader.loadFromFiles("shaders/water.vert", "shaders/water.frag");
	lightShader.loadFromFiles("shaders/lighting.vert", "shaders/lighting.frag");
	shadowShader.loadFromFiles("shaders/shadow.vert", "shaders/shadow.frag");
	simpleShader.loadFromFiles("shaders/simple.vert", "shaders/simple.frag");
	skyboxShader.loadFromFiles("shaders/skybox.vert", "shaders/skybox.frag");
	terrainShader.loadFromFiles("shaders/terrain.vert", "shaders/terrain.frag");
	canvasShader.loadFromFiles("shaders/canvas.vert", "shaders/canvas.frag");
	particleShader.loadFromFiles("shaders/particle.vert", "shaders/particle.frag");
	lineShader.loadFromFiles("shaders/lines.vert", "shaders/lines.frag");
	pickingShader.loadFromFiles("shaders/picking.vert", "shaders/picking.frag");
	shadowPointLightShader.loadFromFiles("shaders/shadow_point_light.vert", "shaders/shadow_point_light.frag");

	skyboxDay.loadFromFiles({
		"data/skybox/ClearOcean/vz_clear_ocean_right.png",
		"data/skybox/ClearOcean/vz_clear_ocean_left.png",
		"data/skybox/ClearOcean/vz_clear_ocean_up.png",
		"data/skybox/ClearOcean/vz_clear_ocean_down.png",
		"data/skybox/ClearOcean/vz_clear_ocean_front.png",
		"data/skybox/ClearOcean/vz_clear_ocean_back.png"
	});
	skyboxDusk.loadFromFiles({
		"data/skybox/DuskOcean/vz_dusk_ocean_right.png",
		"data/skybox/DuskOcean/vz_dusk_ocean_left.png",
		"data/skybox/DuskOcean/vz_dusk_ocean_up.png",
		"data/skybox/DuskOcean/vz_dusk_ocean_down.png",
		"data/skybox/DuskOcean/vz_dusk_ocean_front.png",
		"data/skybox/DuskOcean/vz_dusk_ocean_back.png"
	});
	skyboxNight.loadFromFiles({
		"data/skybox/SinisterOcean/vz_sinister_ocean_right.png",
		"data/skybox/SinisterOcean/vz_sinister_ocean_left.png",
		"data/skybox/SinisterOcean/vz_sinister_ocean_up.png",
		"data/skybox/SinisterOcean/vz_sinister_ocean_down.png",
		"data/skybox/SinisterOcean/vz_sinister_ocean_front.png",
		"data/skybox/SinisterOcean/vz_sinister_ocean_back.png"
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

	particleTechnique.setShader(&particleShader);
	particleTechnique.init();

	pickingTechnique.setShader(&pickingShader);
	pickingTechnique.init();

	cubemapShadowMapTechnique.setShader(&shadowPointLightShader);
	cubemapShadowMapTechnique.init();

	particleSpritesheet = new Spritesheet();
	particleSpritesheet->tex = AssetManager::get()->getTexture("data/particles/flashburst_spritesheet.png");
	particleSpritesheet->cols = 5;
	particleSpritesheet->numSprites = 20;
	particleSpritesheet->spriteWidth = 192;
	particleSpritesheet->spriteHeight = 192;

	particleAnim = new ParticleAnim();
	particleAnim->spritesheet = particleSpritesheet;
	particleAnim->frames = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

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

	lineRenderer.shader = &lineShader;
	lineRenderer.init();

	mainCamera.position = glm::vec3(WORLD_SIZE * 0.5f, WORLD_MAX_HEIGHT * 0.8f, WORLD_SIZE * 0.5f - 10);
	mainCamera.direction = glm::vec3(0.0f, -0.5f, 1.0f);
	mainCamera.Near = 0.1f;
	mainCamera.Far = 1000.0f;

	lightCamera.viewport.width = 64;
	lightCamera.viewport.height = 64;
	lightCamera.Near = 0.1f;
	lightCamera.Far = 100.0f;
	lightCamera.updateProjection();

	pointLightCamera.viewport.width = 1024;
	pointLightCamera.viewport.height = 1024;
	pointLightCamera.fov = AI_MATH_PI;
	pointLightCamera.Near = 1.0f;
	pointLightCamera.Far = 100.0f;
	pointLightCamera.updateProjection();

	fog.Color = glm::vec3(0.9f, 0.9f, 0.95f);
	fog.Density = 0.005f;

	dirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
	dirLight.diffuseIntensity = 0.9f;
	dirLight.ambientIntensity = 0.1f;
	dirLight.worldDirection = glm::vec3(0.0f, -2.0f, -1.0f);

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
	terrainMaterial.diffuseTexture = AssetManager::get()->getTexture("data/terrain/grass_green_d.jpg");
	terrainMaterial.normalMap = AssetManager::get()->getTexture("data/terrain/grass_green_n.jpg");
	terrainMaterial.diffuseTextureRed = AssetManager::get()->getTexture("data/terrain/desert_sand_d.jpg");
	terrainMaterial.normalMapRed = AssetManager::get()->getTexture("data/terrain/desert_sand_n.jpg");
	terrainMaterial.diffuseTextureGreen = AssetManager::get()->getTexture("data/terrain/mntn_brown_d.jpg");
	terrainMaterial.normalMapGreen = AssetManager::get()->getTexture("data/terrain/mntn_brown_n.jpg");
	terrainMaterial.diffuseTextureBlue = AssetManager::get()->getTexture("data/terrain/snow1_d.jpg");
	terrainMaterial.normalMapBlue = AssetManager::get()->getTexture("data/terrain/snow1_n.jpg");

	terrainManager = new TerrainManager({3, 3}, WORLD_SIZE / 3, &terrainMaterial);

	Terrain::HeightMapConfig heightmapConfig;
	heightmapConfig.maxHeight = WORLD_MAX_HEIGHT;
	for (int x = 1; x <= 1; ++x) {
		for (int y = 1; y <= 1; ++y) {
			terrainManager->getTerrain(x, y)->setHeightMap(AssetManager::get()->getTexture("data/heightmaps/wgen_x0_y0.png"), heightmapConfig);
		}
	}

	editor = new Editor(&uiElements, &lineRenderer, terrainManager, &mainCamera);
	editor->initSprites(simpleTechnique);

	WaterTile waterTile;
	waterTile.width = WORLD_SIZE;
	waterTile.height = WORLD_SIZE;

	waterTile.init();

	waterTile.position.x = WORLD_SIZE * 0.5f;
	waterTile.position.y = WORLD_MAX_HEIGHT * 0.16f;
	waterTile.position.z = WORLD_SIZE * 0.5f;

	waterMatrial.distortionIntensity = 0.004f;
	waterMatrial.distortionTexture = AssetManager::get()->getTexture("data/water/waterdudv.jpg");

	waterTile.setMaterial(&waterMatrial);

	waterTiles.push_back(waterTile);

	for (auto& tile : waterTiles) {
		tile.computeModelMatrix();
	}

	Player* player = new Player(&mainCamera);
	player->getMesh()->position.x = WORLD_SIZE * 0.5f;
	player->getMesh()->position.z = WORLD_SIZE * 0.5f;
	player->getMesh()->position.y = terrainManager->getHeightAtPosition(player->getMesh()->position);
	addGameObject(player);

	playerCamera = new PlayerCamera(player, &mainCamera);
	playerCamera->enabled = false;

	grassMaterial.ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	grassMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	grassMaterial.diffuseTexture = AssetManager::get()->getTexture("data/grass/n_grass_atlas.png");

	grassSpritesheet = new Spritesheet();
	grassSpritesheet->cols = 8;
	grassSpritesheet->numSprites = 64;
	grassSpritesheet->tex = AssetManager::get()->getTexture("data/grass/n_grass_atlas.png");
	grassSpritesheet->spriteWidth = 128;
	grassSpritesheet->spriteHeight = 128;

	spline = new Spline();
	spline->points = generateFigureEightPoints(16, 10.0f);
	spline->recalculateSegments();

	threeTowersSpline = new Spline();

	grassBatch.init();
	for (unsigned int i = 0; i < 4000; ++i) {
		int x = rand() % 3;
		int y = rand() % 8;

		SpriteRegion region = grassSpritesheet->getRegion(x + y * grassSpritesheet->cols);
		
		Grass grass;
		grass.material = &grassMaterial;

		grass.size = 1.5f;
		grass.regionX = region.x;
		grass.regionY = region.y;
		grass.regionWidth = region.w;
		grass.regionHeight = region.h;

		grass.position = glm::vec3(random() * WORLD_SIZE, 0, random() * WORLD_SIZE);
		grass.position.y = terrainManager->getHeightAtPosition(grass.position) + grass.size * 0.5f;

		if (grass.position.y > WORLD_MAX_HEIGHT * 0.2f && grass.position.y < WORLD_MAX_HEIGHT * 0.8f) {
			grass.place(grassBatch);
		}
		else {
			--i;
		}
		
	}
	grassBatch.prepare();

	setIsPlaying(false);
	editor->loadLevel(*this);

	std::vector<GameObject*> towers = getObjectsByType("magetower");
	for (unsigned int i = 0; i < towers.size(); ++i) {
		const glm::vec3& pos = towers[i]->getMesh()->position;
		glm::vec3 p = pos + (pos - glm::vec3(WORLD_SIZE * 0.5f, 0, WORLD_SIZE * 0.5f)) * 0.1f + glm::vec3(0, WORLD_MAX_HEIGHT * 0.8f, 0);
		threeTowersSpline->points.push_back(p);
	}
	threeTowersSpline->recalculateSegments();
}

void Game::update(float dt)
{
	Window* win = Window::get();

	canvas.beginFrame();
	lineRenderer.beginFrame();

	float angle = 0.3f;

	dirLight.worldDirection.x = cos(angle) * 1.5f;
	dirLight.worldDirection.z = sin(angle) * 1.5f;

	lightCamera.position = getObjectByName("player")->getMesh()->position - dirLight.worldDirection * 10.0f;
	lightCamera.direction = dirLight.worldDirection;

	if (isPlaying) {
		for (GameObject* go : gameObjects) {
			go->update(dt);
		}
		playerCamera->update(dt);
	}

	editor->update(dt);

	if (animating) {
		t += 0.1f * dt;
		if (t >= threeTowersSpline->points.size()) {
			animating = false;
		}
		mainCamera.position = threeTowersSpline->getPoint(t);
		mainCamera.direction = threeTowersSpline->getPoint1stDerivate(t) + glm::vec3(0, -0.5, 0);
	}

	mainCamera.onResize(win->getWinWdth(), win->getWinHeight());
	mainCamera.update();

	lightCamera.update();

	for (Particle& p : particles) {
		p.update(dt);
	}

	auto filter = [](Particle& p) {
		return !p.isAlive();
	};
	particles.erase(std::remove_if(particles.begin(), particles.end(), filter), particles.end());
	Camera* camera = &mainCamera;
	auto comp = [camera](Particle& p1, Particle& p2) {
		return glm::distance(camera->position, p1.position) > glm::distance(camera->position, p2.position);
		};
	std::sort(particles.begin(), particles.end(), comp);

	if (!menuOpened) {
		if (uiElements.button("Menu", 10, 10)) {
			menuOpened = true;
		}
	}
	else {
		uiElements.begin(10, 10, 600, 300);

		float y = 0;

		if (uiElements.button("X", 550, 0, 40, 40)) {
			menuOpened = false;
		}

		if (isPlaying) {
			y += 50.0f;
			uiElements.text("Camera", 0, y);
			y += 40.0f;
			if (uiElements.button("Static", 0, y)) {
				playerCamera->setPlayer(nullptr);
				mainCamera.position = glm::vec3(WORLD_SIZE * 0.35f, WORLD_MAX_HEIGHT * 0.8f, WORLD_SIZE * 0.35f);
				mainCamera.direction = glm::vec3(1.0f, -0.5f, 1.0f);
			}
			if (uiElements.button("Player", 200, y)) {
				playerCamera->setPlayer(getObjectByName("player"));
			}
			y += 50.0f;
			if (uiElements.button("Horse", 0, y)) {
				std::vector<GameObject*> horses = getObjectsByType("horse");
				if (!horses.empty()) {
					playerCamera->setPlayer(horses[0]);
				}
			}
			if (uiElements.button("Animation", 200, y)) {
				playerCamera->setPlayer(nullptr);
				animating = true;
				t = 0.0f;
			}
		}

		y += 50.0f;
		uiElements.text("Day Time", 0, y);
		y += 40.0f;
		if (uiElements.button("Day", 0, y)) {
			setDayTime(DayTime::DAY);
		}
		if (uiElements.button("Dusk", 200, y)) {
			setDayTime(DayTime::DUSK);
		}
		if (uiElements.button("Night", 400, y)) {
			setDayTime(DayTime::NIGHT);
		}

		uiElements.end();
	}

	if (uiElements.button(isPlaying ? "Stop" : "Play", win->getWinWdth() * 0.5f - 80, 10.0f)) {
		setIsPlaying(!isPlaying);
	}

	editor->drawUI(*this);

	if (Input::get()->isMouseButtonClicked(GLUT_LEFT_BUTTON) && Input::get()->isKeyDown('q') && editor->enabled) {
		unsigned int mouseX = Input::get()->getMouseX();
		unsigned int mouseY = Input::get()->getMouseY();

		PickingTechnique::Pixel pixel;
		pickingTechnique.readPixel(mouseX, mouseY, pixel);

		for (GameObject* go : gameObjects) {
			if (go->getMesh()->getID() == pixel.ObjectID) {
				editor->selectGameObject(go);
				break;
			}
		}
	}

	canvas.endFrame();
	lineRenderer.endFrame();
}

void Game::draw()
{
	pickingPhase();

	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	shadowPass();

	Window* win = Window::get();

	glViewport(0, 0, win->getWinWdth(), win->getWinHeight());

	drawScene(glm::vec4(0, -1, 0, 1000), false);

	for (auto& tile : waterTiles) {
		waterTechnique.bindReflectionFBO(tile, mainCamera);

		drawScene(glm::vec4(0, 1, 0, -tile.position.y), true);

		waterTechnique.unbindReflectionFBO(tile, mainCamera);

		waterTechnique.bindRefractionFBO(tile, mainCamera);

		drawScene(glm::vec4(0, -1, 0, tile.position.y), true);

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

	lineRenderer.draw(mainCamera.getPVMatrix());
	canvas.draw();
}

void Game::addParticle(Particle p)
{
	particles.push_back(p);
}

void Game::shadowPass()
{
	shadowMapTechnique.bindFBO();

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	shadowMapTechnique.use();

	glDisable(GL_CULL_FACE);
	for (GameObject* go : gameObjects) {
		shadowMapTechnique.draw(*(go->getMesh()), { &lightCamera });
	}
	glEnable(GL_CULL_FACE);
	const auto& terrains = terrainManager->getTerrains();
	for (auto it = terrains.begin(); it != terrains.end(); ++it) {
		shadowMapTechnique.draw(*(*it).second, { &lightCamera });
	}

	shadowMapTechnique.unbindFBO();

	PointLight* nearestPointLight = nullptr;
	float minDist = FLT_MAX;
	for (PointLight* light : pointLights) {
		float dist = glm::distance(light->position, mainCamera.position);
		if (dist < minDist) {
			minDist = dist;
			nearestPointLight = light;
		}
	}

	if (nearestPointLight) {

		pointLightCamera.position = nearestPointLight->position;

		glEnable(GL_DEPTH_TEST);
		glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

		cubemapShadowMapTechnique.use();

		cubemapShadowMapTechnique.supplyLightPos(pointLightCamera.position);

		for (unsigned int i = 0; i < 6; ++i) {
			const CubemapShadowMapTechnique::CameraDirection& cameraDir = cubemapShadowMapTechnique.cameraDirections[i];

			cubemapShadowMapTechnique.bindCubemapFBO(cameraDir.cubemapUnit);

			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			pointLightCamera.direction = cameraDir.direction;
			pointLightCamera.up = cameraDir.up;
			pointLightCamera.update();

			glDisable(GL_CULL_FACE);
			for (GameObject* go : gameObjects) {
				cubemapShadowMapTechnique.draw(*(go->getMesh()), { &pointLightCamera });
			}
			glEnable(GL_CULL_FACE);
			const auto& terrains = terrainManager->getTerrains();
			for (auto it = terrains.begin(); it != terrains.end(); ++it) {
				cubemapShadowMapTechnique.draw(*(*it).second, { &pointLightCamera });
			}
		}

		cubemapShadowMapTechnique.unbindCubemapFBO();
	}
}

void Game::drawScene(const glm::vec4& clipPlane, bool transparent)
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
	cubemapShadowMapTechnique.bindCubemapShadowMap();

	glDisable(GL_CULL_FACE);
	
	for (GameObject* go : gameObjects) {
		lightingTechnique.draw(*(go->getMesh()), {&mainCamera, &lightCamera});
	}

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
	cubemapShadowMapTechnique.bindCubemapShadowMap();

	const auto& terrains = terrainManager->getTerrains();
	for (auto it = terrains.begin(); it != terrains.end(); ++it) {
		terrainTechnique.draw(*(*it).second, {&mainCamera, &lightCamera});
	}

	glCullFace(GL_FRONT);

	cubemapTechnique.use();
	bindSkybox();
	cubemapTechnique.supplyPVMatrix(mainCamera.getProjMatrix() * glm::mat4(glm::mat3(mainCamera.getViewMatrix())));
	cubemapTechnique.draw();

	glCullFace(GL_BACK);

	if (transparent) {
		drawTransparent(clipPlane);
	}
}

void Game::drawTransparent(const glm::vec4& clipPlane)
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

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);

	particleTechnique.use();
	particleTechnique.supplySpritesheetInfo(*particleSpritesheet);
	particleTechnique.draw(particles, &mainCamera);

	glDepthMask(true);
	glDisable(GL_BLEND);
}

void Game::bindSkybox()
{
	switch (dayTime) {
	case DayTime::DAY:
		skyboxDay.bind(DIFFUSE);
		break;
	case DayTime::DUSK:
		skyboxDusk.bind(DIFFUSE);
		break;
	case DayTime::NIGHT:
		skyboxNight.bind(DIFFUSE);
		break;
	}
}

void Game::setDayTime(DayTime dayTime)
{
	if (dayTime == this->dayTime) return;

	this->dayTime = dayTime;
	switch (dayTime) {
	case DayTime::DAY:
		dirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
		dirLight.diffuseIntensity = 1.0f;
		dirLight.ambientIntensity = 0.1f;
		dirLight.worldDirection = glm::vec3(0.0f, -2.0f, -1.0f);

		fog.Color = glm::vec3(0.8f, 0.85f, 0.95f);
		fog.Density = 0.005f;
		break;
	case DayTime::DUSK:
		dirLight.color = glm::vec3(0.5f, 0.4f, 0.4f);
		dirLight.diffuseIntensity = 1.0f;
		dirLight.ambientIntensity = 0.1f;
		dirLight.worldDirection = glm::vec3(0.0f, -2.0f, -1.0f);

		fog.Color = glm::vec3(0.5f, 0.4f, 0.4f);
		fog.Density = 0.005f;
		break;
	case DayTime::NIGHT:
		dirLight.color = glm::vec3(0.1f, 0.1f, 0.2f);
		dirLight.diffuseIntensity = 1.0f;
		dirLight.ambientIntensity = 0.1f;
		dirLight.worldDirection = glm::vec3(0.0f, -2.0f, -1.0f);

		fog.Color = glm::vec3(0.1f, 0.1f, 0.2f);
		fog.Density = 0.005f;
		break;
	}
}

void Game::pickingPhase()
{
	pickingTechnique.use();
	pickingTechnique.enableWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (GameObject* go : gameObjects) {
		pickingTechnique.draw(*(go->getMesh()), {&mainCamera});
	}

	pickingTechnique.disableWriting();
}

std::vector<glm::vec3> Game::generateFigureEightPoints(int count, float size)
{
	std::vector<glm::vec3> points;
	points.reserve(count);

	for (int i = 0; i < count; ++i) {
		float t = 2.0f * AI_MATH_TWO_PI * float(i) / float(count - 1);
		float x = size * std::sin(t);
		float y = size * std::sin(t) * std::cos(t);
		points.emplace_back(x, 0, y);
	}

	return points;
}

void Game::setIsPlaying(bool value)
{
	isPlaying = value;
	Player* player = (Player*)getObjectByName("player");
	player->enableMovement(isPlaying);
	playerCamera->enabled = isPlaying;
	editor->enable(!isPlaying);
	if (!isPlaying) {
		animating = false;
		for (GameObject* go : gameObjects) {
			if (go->type == "horse") {
				go->getMesh()->position = ((Horse*)go)->initialPosition;
			}
		}
	}
}
