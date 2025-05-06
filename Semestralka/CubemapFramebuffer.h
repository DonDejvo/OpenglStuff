#pragma once

#include "Framebuffer.h"

class CubemapFramebuffer : public Framebuffer {
private:
	GLuint mCubeMap;
	unsigned int cubemapAttachmentIdx;
public:
	void createCubemapAttachment(GLint internalFormat, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR);
	void bindCubemap(GLuint cubemapUnit, GLenum unit = GL_FRAMEBUFFER) const;
	void bindCubemapTexture(GLenum texUnit) const;
};