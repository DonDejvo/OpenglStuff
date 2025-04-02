#include "ShaderTechnique.h"

void ShaderTechnique::init()
{
	PVMLocation = glGetUniformLocation(mShader->getProgramID(), "u_PVM");
	modelLocation = glGetUniformLocation(mShader->getProgramID(), "u_Model");
}

void ShaderTechnique::setShader(Shader* shader)
{
	mShader = shader;
}

void ShaderTechnique::use() const
{
	mShader->use();
}

