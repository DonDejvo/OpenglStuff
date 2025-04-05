#include "ShaderTechnique.h"

void ShaderTechnique::init()
{
}

void ShaderTechnique::setShader(Shader* shader)
{
	mShader = shader;
}

void ShaderTechnique::use() const
{
	mShader->use();
}

