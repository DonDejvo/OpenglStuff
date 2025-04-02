#pragma once

#include "Game.h"
#include "LightingTechnique.h"
#include "ShadowMapFBO.h"
#include "Geometry.h"
#include "Mesh.h"

class LightDemo : public Game {
public:
	using Game::Game;

	PerspectiveCamera mainCamera;
	Shader lightShader, shadowShader;
	LightingTechnique lightingTechnique;

	DirectionalLight dirLight;
	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;

	QuadGeometry quadGeometry;
	PlaneGeometry planeGeometry;

	Mesh backpack;
	Mesh plane;

	ShadowMapFBO shadowMapFBO;

	void init() override;
	void update(float dt) override;
	void draw() const override;
};