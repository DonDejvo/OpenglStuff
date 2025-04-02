#include "LightDemo.h"
#include "Window.h"

void LightDemo::init()
{
	lightShader.loadFromFiles("shaders/lighting.vert", "shaders/lighting.frag");
	shadowShader.loadFromFiles("shaders/shadow.vert", "shaders/shadow.frag");

	lightingTechnique.setShader(&lightShader);
	lightingTechnique.init();

	mainCamera.position = glm::vec3(0.0f, 5.0f, 10.0f);
	mainCamera.direction = glm::vec3(0.0f, -0.5f, -2.0f);

	dirLight.ambientIntensity = 0.1f;
	dirLight.diffuseIntensity = 0.5f;
	dirLight.worldDirection = glm::vec3(0.0f, -2.0f, -1.0f);

	PointLight light1;
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
	spotLights.push_back(light4);

	backpack.loadFromFile("data/backpack/backpack.obj");

	backpack.position.y = 2.5f;

	planeGeometry.init();
	planeGeometry.initBuffers();

	quadGeometry.init();
	quadGeometry.initBuffers();

	plane.setGeometry(&planeGeometry);
	Material colorMaterial;
	colorMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	plane.setMaterial(0, colorMaterial);

	plane.scale *= 30.0f;

	shadowMapFBO.init();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void LightDemo::update(float dt)
{
	Window* win = Window::get();
	mainCamera.onResize(win->getWinWdth(), win->getWinHeight());
	mainCamera.update(dt);

	backpack.yaw += 1.0f * dt;

	backpack.computeModelMatrix();
	plane.computeModelMatrix();
}

void LightDemo::draw() const
{
	Window* win = Window::get();

	glViewport(0, 0, win->getWinWdth(), win->getWinHeight());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lightingTechnique.use();

	lightingTechnique.bindTextureUnits();

	lightingTechnique.supplyCameraPosition(mainCamera.position);
	lightingTechnique.supplyDirLight(dirLight);
	lightingTechnique.supplyPointLights(pointLights);
	lightingTechnique.supplySpotLights(spotLights);

	lightingTechnique.supplyModelMatrix(backpack.getMatrix());
	glm::mat4 PVMMatrix = mainCamera.getPVMatrix() * backpack.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	backpack.draw((DrawCallbacks*)&lightingTechnique);

	lightingTechnique.supplyModelMatrix(plane.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * plane.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	plane.draw((DrawCallbacks*)&lightingTechnique);
}
