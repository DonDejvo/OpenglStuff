#include "WaterTechnique.h"

void WaterTechnique::init()
{
	ShaderTechnique::init();

	mReflectionFramebuffer.width = 1024;
	mReflectionFramebuffer.height = 1024;
	mReflectionFramebuffer.init();
	mReflectionFramebuffer.createAttachment(GL_RGB);
	mReflectionFramebuffer.createDepthBuffer();

	mRefractionFramebuffer.width = 1024;
	mRefractionFramebuffer.height = 1024;
	mRefractionFramebuffer.init();
	mRefractionFramebuffer.createAttachment(GL_RGB);
	mRefractionFramebuffer.createDepthBuffer();

	mTextureLoc[REFLECTION - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_TextureReflection");
	mTextureLoc[REFRACTION - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_TextureRefraction");
}
