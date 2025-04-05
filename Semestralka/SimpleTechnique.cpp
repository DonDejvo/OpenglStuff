#include "SimpleTechnique.h"

void SimpleTechnique::init()
{
	ShaderTechnique::init();

	PVMLocation = glGetUniformLocation(mShader->getProgramID(), "u_PVM");
}

void SimpleTechnique::supplyPVMMatrix(const glm::mat4& PVMMatrix) const
{
	glUniformMatrix4fv(PVMLocation, 1, GL_FALSE, &PVMMatrix[0][0]);
}
