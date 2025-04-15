#include "Framebuffer.h"

void Framebuffer::init()
{
	glGenFramebuffers(1, &mFBO);
}

void Framebuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

void Framebuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::createAttachment(GLenum internalFormat, GLenum minFilter, GLenum magFilter)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	Texture colorAttachment;
	colorAttachment.create(width, height, internalFormat, GL_UNSIGNED_BYTE, minFilter, magFilter);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + mColorAttachments.size(), GL_TEXTURE_2D, colorAttachment.getTexID(), 0);

	mColorAttachments.push_back(colorAttachment);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::createDepthAttachment(GLenum internalFormat, GLenum minFilter, GLenum magFilter)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	mDepthAttachment.create(width, height, internalFormat, GL_FLOAT, minFilter, magFilter);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment.getTexID(), 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::createDepthBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

	glGenRenderbuffers(1, &mRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const Texture& Framebuffer::getAttachment(unsigned int idx) const
{
	return mColorAttachments[idx];
}

const Texture& Framebuffer::getDepthAttachment() const
{
	return mDepthAttachment;
}
