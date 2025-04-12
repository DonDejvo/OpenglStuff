#include "ShadowMapFBO.h"
#include <iostream>

ShadowMapFBO::ShadowMapFBO()
{
	mFbo = 0;
	width = 1024;
	height = 1024;
}

void ShadowMapFBO::init()
{
	glGenFramebuffers(1, &mFbo);
	
	Texture::CreateParams params;
	params.format = GL_DEPTH_COMPONENT;
	params.type = GL_FLOAT;
	params.filter = GL_NEAREST;
	params.wrap = GL_CLAMP_TO_EDGE;
	mShadowMap.create(width, height, params);

	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap.getTexID(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer OK\n";
	}
	else {
		std::cout << "ERROR: Framebuffer Failed: " << status << "\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapFBO::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	glViewport(0, 0, width, height);
}

void ShadowMapFBO::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
