#include "ShadowMapFBO.h"
#include <iostream>

ShadowMapFBO::ShadowMapFBO()
{
	width = 1024;
	height = 1024;
}

void ShadowMapFBO::init()
{
	glGenFramebuffers(1, &mFbo);
	
	mShadowMap.create(width, height);
	mShadowMap.bind(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mShadowMap.getWidth(), mShadowMap.getHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindBuffer(GL_FRAMEBUFFER, mFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap.getTexID(), 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer OK\n";
	}
	else {
		std::cout << "Framebuffer Failed: " << status << "\n";
	}

	glBindBuffer(GL_FRAMEBUFFER, 0);
}
