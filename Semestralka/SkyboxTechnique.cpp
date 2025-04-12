#include "SkyboxTechnique.h"

void SkyboxTechnique::init()
{
	ShaderTechnique::init();

	mPVMatrixLoc = glGetUniformLocation(mShader->getProgramID(), "u_PV");
}

void SkyboxTechnique::supplyPVMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(mPVMatrixLoc, 1, GL_FALSE, &matrix[0][0]);
}

void SkyboxTechnique::draw() const
{
	GLint prevCullFaceMode, prevDepthFunc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFunc);

	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(mGeometry->getVAO());
	for (unsigned int i = 0; i < mGeometry->drawCalls.size(); ++i) {
		mGeometry->draw(i);
	}
	glBindVertexArray(0);

	glDepthFunc(prevDepthFunc);
}
