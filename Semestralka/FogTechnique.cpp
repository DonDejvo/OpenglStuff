#include "FogTechnique.h"

void FogTechnique::init()
{
	ShaderTechnique::init();

	mFogEnabledLoc = glGetUniformLocation(mShader->getProgramID(), "u_FogEnabled");

	mFogLoc.Density = glGetUniformLocation(mShader->getProgramID(), "u_Fog.Density");
	mFogLoc.Gradient = glGetUniformLocation(mShader->getProgramID(), "u_Fog.Gradient");
	mFogLoc.Color = glGetUniformLocation(mShader->getProgramID(), "u_Fog.Color");
}

void FogTechnique::enableFog(bool value) const
{
	glUniform1i(mFogEnabledLoc, value);
}

void FogTechnique::supplyFog(const Fog& fog) const
{
	glUniform1f(mFogLoc.Density, fog.Density);
	glUniform1f(mFogLoc.Gradient, fog.Gradient);
	glUniform3fv(mFogLoc.Color, 1, &fog.Color[0]);
}

Fog::Fog()
{
	Density = 0.0f;
	Gradient = 2.0f;
	Color = glm::vec3(0.0f);
}
