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
	mShadowMap.create(width, height, params);

	glBindBuffer(GL_FRAMEBUFFER, mFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mShadowMap.getTexID(), 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer OK\n";
	}
	else {
		std::cout << "Framebuffer Failed: " << status << "\n";
	}

	glBindBuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapFBO::bind() const
{
	glBindBuffer(GL_FRAMEBUFFER, mFbo);
	glViewport(0, 0, width, height);
}

void ShadowMapFBO::unbind() const
{
	glBindBuffer(GL_FRAMEBUFFER, 0);
}
