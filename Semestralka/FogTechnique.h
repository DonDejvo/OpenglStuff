#pragma once

#include "ShaderTechnique.h"

struct Fog {
	float Density;
	float Gradient;
	glm::vec3 Color;

	Fog();
};

class FogTechnique : public ShaderTechnique {
protected:
	GLuint mFogEnabledLoc;
	struct {
		GLuint Density;
		GLuint Gradient;
		GLuint Color;
	} mFogLoc;
public:
	void init() override;
	void enableFog(bool value) const;
	void supplyFog(const Fog& fog) const;
};