#include "LightDemo.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include "AssetManager.h"

void LightDemo::init()
{
	lightShader.loadFromFiles("shaders/lighting.vert", "shaders/lighting.frag");
	shadowShader.loadFromFiles("shaders/shadow.vert", "shaders/shadow.frag");
	screenShader.loadFromFiles("shaders/screen.vert", "shaders/screen.frag");
	simpleShader.loadFromFiles("shaders/simple.vert", "shaders/simple.frag");

	lightingTechnique.setShader(&lightShader);
	lightingTechnique.init();

	shadowMapTechnique.setShader(&shadowShader);
	shadowMapTechnique.init();

	screenTechnique.setShader(&screenShader);
	screenTechnique.init();

	simpleTechnique.setShader(&simpleShader);
	simpleTechnique.init();

	mainCamera.position = glm::vec3(0.0f, 5.0f, 10.0f);
	mainCamera.direction = glm::vec3(0.0f, -0.5f, -2.0f);

	lightCamera.position = glm::vec3(0.0f, 20.0f, 10.0f);
	lightCamera.direction = glm::vec3(0.0f, -2.0f, -1.0f);
	lightCamera.viewport.width = 25;
	lightCamera.viewport.height = 25;
	lightCamera.Near = 1.0f;
	lightCamera.Far = 100.0f;
	lightCamera.updateProjection();

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

	colorMaterial.ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	colorMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);

	backpack.loadFromFile("data/backpack/backpack.obj");
	/*backpack.scale *= 3.0f;
	backpack.setGeometry(&cubeGeometry);
	backpack.setMaterial(0, &colorMaterial);*/
	backpack.position.x = 0.0f;
	backpack.position.y = 2.0f;
	backpack.position.z = -0.5f;

	backpack1.loadFromFile("data/backpack/backpack.obj");
	backpack1.position.x = -4.0f;
	backpack1.position.y = 2.0f;
	backpack1.position.z = -2.5f;

	plane.setGeometry(&planeGeometry);
	plane.setMaterial(0, &colorMaterial);

	plane.scale *= 30.0f;

	cube.setGeometry(&cubeGeometry);
	cube.setMaterial(0, &colorMaterial);
	cube.scale *= 2.0f;
	cube.scale.y *= 2.0f;
	cube.position.x = 4.0f;
	cube.position.y = 2.0f;
	cube.position.z = -2.5f;

	/*shadowMapFBO.width = 1024;
	shadowMapFBO.height = 1024;
	shadowMapFBO.init();*/

	Texture::CreateParams params;
	params.format = GL_DEPTH_COMPONENT;
	params.type = GL_FLOAT;
	params.filter = GL_NEAREST;
	framebufferTexture.create(1024, 1024, params);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, framebufferTexture.getTexID(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	/*glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);*/

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	float angle = (dirLightAngle < AI_MATH_PI ? dirLightAngle : AI_MATH_TWO_PI - dirLightAngle) + AI_MATH_PI;

	angle = 5.7f;

	dirLight.worldDirection.x = cos(angle) * 3.0f;
	dirLight.worldDirection.z = sin(angle) * 3.0f;

	lightCamera.position = glm::vec3(-dirLight.worldDirection.x * 5.0f, -dirLight.worldDirection.y * 5.0f, -dirLight.worldDirection.z * 5.0f);
	lightCamera.direction = glm::vec3(dirLight.worldDirection);

	backpack.computeModelMatrix();
	backpack1.computeModelMatrix();
	plane.computeModelMatrix();
	cube.computeModelMatrix();

	mainCamera.onResize(win->getWinWdth(), win->getWinHeight());
	mainCamera.update(dt);

	lightCamera.update(dt);
}

void LightDemo::draw()
{
	/*glm::mat4 PVMMatrix;

	shadowMapFBO.bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shadowMapTechnique.use();

	PVMMatrix = lightCamera.getPVMatrix() * backpack.getMatrix();
	shadowMapTechnique.supplyPVMMatrix(PVMMatrix);

	backpack.draw((DrawCallbacks*)&shadowMapTechnique);

	shadowMapFBO.unbind();

	Window* win = Window::get();

	glViewport(0, 0, win->getWinWdth(), win->getWinHeight());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lightingTechnique.use();

	lightingTechnique.bindTextureUnits();

	lightingTechnique.supplyCameraPosition(mainCamera.position);
	lightingTechnique.supplyDirLight(dirLight);
	lightingTechnique.supplyPointLights(pointLights);
	lightingTechnique.supplySpotLights(spotLights);

	shadowMapFBO.getShadowMap().bind(GL_TEXTURE2);

	lightingTechnique.supplyModelMatrix(backpack.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * backpack.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	backpack.draw((DrawCallbacks*)&lightingTechnique);

	lightingTechnique.supplyModelMatrix(plane.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * plane.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	plane.draw((DrawCallbacks*)&lightingTechnique);*/

	glm::mat4 PVMMatrix, lightPVMMatrix;

	//shadowMapFBO.bind();

	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	shadowMapTechnique.use();

	lightPVMMatrix = lightCamera.getPVMatrix() * backpack.getMatrix();
	shadowMapTechnique.supplyPVMMatrix(lightPVMMatrix);
	backpack.draw((DrawCallbacks*)&shadowMapTechnique);

	lightPVMMatrix = lightCamera.getPVMatrix() * backpack1.getMatrix();
	shadowMapTechnique.supplyPVMMatrix(lightPVMMatrix);
	backpack1.draw((DrawCallbacks*)&shadowMapTechnique);

	lightPVMMatrix = lightCamera.getPVMatrix() * cube.getMatrix();
	shadowMapTechnique.supplyPVMMatrix(lightPVMMatrix);
	cube.draw((DrawCallbacks*)&shadowMapTechnique);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//shadowMapFBO.unbind();

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

	framebufferTexture.bind(GL_TEXTURE2);

	lightingTechnique.supplyModelMatrix(backpack.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * backpack.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	lightPVMMatrix = lightCamera.getPVMatrix() * backpack.getMatrix();
	lightingTechnique.supplyLightPVMMatrix(lightPVMMatrix);
	backpack.draw((DrawCallbacks*)&lightingTechnique);

	lightingTechnique.supplyModelMatrix(backpack1.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * backpack1.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	lightPVMMatrix = lightCamera.getPVMatrix() * backpack1.getMatrix();
	lightingTechnique.supplyLightPVMMatrix(lightPVMMatrix);
	backpack1.draw((DrawCallbacks*)&lightingTechnique);

	lightingTechnique.supplyModelMatrix(cube.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * cube.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	lightPVMMatrix = lightCamera.getPVMatrix() * cube.getMatrix();
	lightingTechnique.supplyLightPVMMatrix(lightPVMMatrix);
	cube.draw((DrawCallbacks*)&lightingTechnique);

	lightingTechnique.supplyModelMatrix(plane.getMatrix());
	PVMMatrix = mainCamera.getPVMatrix() * plane.getMatrix();
	lightingTechnique.supplyPVMMatrix(PVMMatrix);
	lightPVMMatrix = lightCamera.getPVMatrix() * plane.getMatrix();
	lightingTechnique.supplyLightPVMMatrix(lightPVMMatrix);
	plane.draw((DrawCallbacks*)&lightingTechnique);
}
