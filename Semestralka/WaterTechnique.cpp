#include "WaterTechnique.h"

void WaterTechnique::init()
{
	ShaderTechnique::init();

	mReflectionFramebuffer.width = 1024;
	mReflectionFramebuffer.height = 1024;
	mReflectionFramebuffer.init();
	mReflectionFramebuffer.createAttachment(GL_RGB8);
	mReflectionFramebuffer.createDepthBuffer();

	mRefractionFramebuffer.width = 1024;
	mRefractionFramebuffer.height = 1024;
	mRefractionFramebuffer.init();
	mRefractionFramebuffer.createAttachment(GL_RGB8);
	mRefractionFramebuffer.createDepthBuffer();

	modelLocation = glGetUniformLocation(mShader->getProgramID(), "u_Model");
	mCameraPosLoc = glGetUniformLocation(mShader->getProgramID(), "u_CameraPosition");

	mTextureLoc[REFLECTION - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_TextureReflection");
	mTextureLoc[REFRACTION - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_TextureRefraction");
	mTextureLoc[DISTORTION_MAP - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_DistortionMap");
	mTextureLoc[TEST_TEX - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_TestTex");

	mDistortionIntensity = glGetUniformLocation(mShader->getProgramID(), "u_DistortionIntensity");

	mDistortionEnabledLoc = glGetUniformLocation(mShader->getProgramID(), "u_DistortionEnabled");

	mTimeLoc = glGetUniformLocation(mShader->getProgramID(), "u_Time");

	testLoc = glGetUniformLocation(mShader->getProgramID(), "u_Test");
	cameraPositionTestLoc = glGetUniformLocation(mShader->getProgramID(), "u_CameraPositionTest");
}

void WaterTechnique::bindReflectionFBO(WaterTile& tile, Camera& camera) const
{
	camera.position.y -= (camera.position.y - tile.position.y) * 2;
	camera.direction.y *= -1;
	camera.update();

	glViewport(0, 0, mReflectionFramebuffer.width, mReflectionFramebuffer.height);
	mReflectionFramebuffer.bind();
}

void WaterTechnique::unbindReflectionFBO(WaterTile& tile, Camera& camera) const
{
	mReflectionFramebuffer.unbind();

	camera.position.y += (tile.position.y - camera.position.y) * 2;
	camera.direction.y *= -1;
	camera.update();
}

void WaterTechnique::bindRefractionFBO(WaterTile& tile, Camera& camera) const
{
	glViewport(0, 0, mRefractionFramebuffer.width, mRefractionFramebuffer.height);
	mRefractionFramebuffer.bind();
}

void WaterTechnique::unbindRefractionFBO(WaterTile& tile, Camera& camera) const
{
	mRefractionFramebuffer.unbind();
}

void WaterTechnique::draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const
{
	mReflectionFramebuffer.getAttachment(0).bind(REFLECTION);
	mRefractionFramebuffer.getAttachment(0).bind(REFRACTION);

	glm::mat4 PVMMatrix = cameras[0]->getPVMatrix() * drawable.getMatrix();
	supplyPVMMatrix(PVMMatrix);
	supplyModelMatrix(drawable.getMatrix());
	supplyCameraPosition(cameras[0]->position);
	drawable.draw((DrawCallbacks*)this);
}

void WaterTechnique::supplyModelMatrix(const glm::mat4& modelMatrix) const
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
}

void WaterTechnique::supplyCameraPosition(const glm::vec3& pos) const
{
	glUniform3fv(cameraPositionTestLoc, 1, &pos[0]);
}

void WaterTechnique::supplyMaterial(const Material& material) const
{
	const WaterMaterrial& waterMaterial = ((WaterMaterrial&)material);
	glUniform1f(mDistortionIntensity, waterMaterial.distortionIntensity);
}

void WaterTechnique::enableDistortion(bool value) const
{
	glUniform1i(mDistortionEnabledLoc, value ? 1 : 0);
}

void WaterTechnique::supplyTime(float time) const
{
	glUniform1f(mTimeLoc, time);
}

void WaterTechnique::supplyTest(float test) const
{
	glUniform1f(testLoc, test);
}
