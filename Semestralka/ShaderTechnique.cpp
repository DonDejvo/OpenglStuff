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

