#include "ShadowMapTechnique.h"

void ShadowMapTechnique::init()
{
	ShaderTechnique::init();

	mPVMMatrixLoc = glGetUniformLocation(mShader->getProgramID(), "u_PVM");
}
