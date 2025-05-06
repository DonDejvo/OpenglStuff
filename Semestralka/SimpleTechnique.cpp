#include "SimpleTechnique.h"

void SimpleTechnique::init()
{
	ShaderTechnique::init();

	PVMLocation = glGetUniformLocation(mShader->getProgramID(), "u_PVM");
	mColorLoc = glGetUniformLocation(mShader->getProgramID(), "u_Color");
	mDiffuseColorLoc = glGetUniformLocation(mShader->getProgramID(), "u_DiffuseColor");
	uDiffuseEnabledLoc = glGetUniformLocation(mShader->getProgramID(), "u_DiffuseEnabled");
}

void SimpleTechnique::supplyPVMMatrix(const glm::mat4& PVMMatrix) const
{
	glUniformMatrix4fv(PVMLocation, 1, GL_FALSE, &PVMMatrix[0][0]);
}

void SimpleTechnique::draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const
{
	glm::mat4 matrix = cameras[0]->getPVMatrix() * drawable.getMatrix();
	supplyPVMMatrix(matrix);
	drawable.draw((DrawCallbacks*)this);
}

void SimpleTechnique::supplyMaterial(const Material& material) const
{
	glUniform3fv(mDiffuseColorLoc, 1, &material.diffuseColor[0]);
}

void SimpleTechnique::enableDiffuseTexture(bool value) const
{
	glUniform1i(uDiffuseEnabledLoc, value ? 1 : 0);
}
