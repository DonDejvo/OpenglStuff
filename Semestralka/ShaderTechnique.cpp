#include "ShaderTechnique.h"

void ShaderTechnique::init()
{
	PVMLocation = glGetUniformLocation(mShader->getProgramID(), "u_PVM");
	modelLocation = glGetUniformLocation(mShader->getProgramID(), "u_Model");
}

ShaderTechnique::ShaderTechnique(Shader* shader): mShader(shader)
{
	init();
}

void ShaderTechnique::use() const
{
	mShader->use();
}

void ShaderTechnique::supplyPVMMatrix(const glm::mat4& PVMMatrix)
{
	glUniformMatrix4fv(PVMLocation, 1, GL_FALSE, &PVMMatrix[0][0]);
}

void ShaderTechnique::supplyModelMatrix(const glm::mat4& modelMatrix)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
}


