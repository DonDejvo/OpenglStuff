#include "ShadowMapTechnique.h"

void ShadowMapTechnique::init()
{
	ShaderTechnique::init();

	mPVMMatrixLoc = glGetUniformLocation(mShader->getProgramID(), "u_PVM");

	mShadowMapFBO.width = 1024;
	mShadowMapFBO.height = 1024;

	mShadowMapFBO.init();
	mShadowMapFBO.createDepthAttachment(GL_DEPTH_COMPONENT16);
}

void ShadowMapTechnique::supplyPVMMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(mPVMMatrixLoc, 1, false, &matrix[0][0]);
}

void ShadowMapTechnique::bindFBO() const
{
	glViewport(0, 0, mShadowMapFBO.width, mShadowMapFBO.height);
	mShadowMapFBO.bind();
}

void ShadowMapTechnique::unbindFBO() const
{
	mShadowMapFBO.unbind();
}

void ShadowMapTechnique::bindShadowMap()
{
	mShadowMapFBO.getDepthAttachment().bind(SHADOW_MAP);
}

void ShadowMapTechnique::draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const
{
	glm::mat4 lightPVMMatrix = cameras[0]->getPVMatrix() * drawable.getMatrix();
	supplyPVMMatrix(lightPVMMatrix);
	drawable.draw((DrawCallbacks*)this);
}
