#include "ShadowMapTechnique.h"

void ShadowMapTechnique::init()
{
	ShaderTechnique::init();

	mPVMMatrixLoc = glGetUniformLocation(mShader->getProgramID(), "u_PVM");
}

void ShadowMapTechnique::supplyPVMMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(mPVMMatrixLoc, 1, false, &matrix[0][0]);
}
