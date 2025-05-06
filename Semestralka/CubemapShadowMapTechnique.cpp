#include "CubemapShadowMapTechnique.h"

void CubemapShadowMapTechnique::init()
{
	ShaderTechnique::init();

	mPVMMatrixLoc = glGetUniformLocation(mShader->getProgramID(), "u_PVM");
	mModelMatrixLoc = glGetUniformLocation(mShader->getProgramID(), "u_Model");
	mLightPosLoc = glGetUniformLocation(mShader->getProgramID(), "u_LightPosition");

	mCubemapShadowMapFBO.width = 1024;
	mCubemapShadowMapFBO.height = 1024;

	mCubemapShadowMapFBO.init();
	mCubemapShadowMapFBO.createDepthBuffer();
	mCubemapShadowMapFBO.createCubemapAttachment(GL_R32F);
}

void CubemapShadowMapTechnique::supplyPVMMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(mPVMMatrixLoc, 1, false, &matrix[0][0]);
}

void CubemapShadowMapTechnique::supplyModelMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(mModelMatrixLoc, 1, false, &matrix[0][0]);
}

void CubemapShadowMapTechnique::supplyLightPos(const glm::vec3& pos) const
{
	glUniform3fv(mPVMMatrixLoc, 1, &pos[0]);
}

void CubemapShadowMapTechnique::bindCubemapFBO(GLenum cubemapUnit) const
{
	glViewport(0, 0, mCubemapShadowMapFBO.width, mCubemapShadowMapFBO.height);
	mCubemapShadowMapFBO.bindCubemap(cubemapUnit);
}

void CubemapShadowMapTechnique::unbindCubemapFBO() const
{
	mCubemapShadowMapFBO.unbind();
}

void CubemapShadowMapTechnique::bindCubemapShadowMap()
{
	mCubemapShadowMapFBO.bindCubemapTexture(CUBE_MAP_SHADOW_MAP);
}

void CubemapShadowMapTechnique::draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const
{
	glm::mat4 lightPVMMatrix = cameras[0]->getPVMatrix() * drawable.getMatrix();
	supplyModelMatrix(cameras[0]->getPVMatrix());
	supplyPVMMatrix(lightPVMMatrix);
	drawable.draw((DrawCallbacks*)this);
}
