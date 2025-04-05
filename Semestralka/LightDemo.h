#pragma once

#include "Game.h"
#include "LightingTechnique.h"
#include "ShadowMapFBO.h"
#include "Geometry.h"
#include "Mesh.h"
#include "ShadowMapTechnique.h"
#include "ScreenTechnique.h"
#include "SimpleTechnique.h"

class LightDemo : public Game {
public:
	using Game::Game;

	PerspectiveCamera mainCamera;
	OrthoCamera lightCamera;
	Shader lightShader, shadowShader, screenShader, simpleShader;
	LightingTechnique lightingTechnique;
	ShadowMapTechnique shadowMapTechnique;
	ScreenTechnique screenTechnique;
	SimpleTechnique simpleTechnique;

	DirectionalLight dirLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	float dirLightAngle = 0.0f;

	QuadGeometry quadGeometry;
	PlaneGeometry planeGeometry;
	CubeGeometry cubeGeometry;
	Material colorMaterial;

	Mesh backpack, backpack1;
	Mesh plane;
	Mesh cube;

	//ShadowMapFBO shadowMapFBO;

	GLuint FBO, RBO;
	Texture framebufferTexture;

	void init() override;
	void update(float dt) override;
	void draw() override;
};