#pragma once
#include "pgr.h"
#include "Texture.h"
#include <map>

class Framebuffer {
protected:
	GLuint mFBO;
	std::map<unsigned int, Texture> mColorAttachments;
	unsigned int attachmentCount = 0;
	Texture mDepthAttachment;
	GLuint mRBO;
public:
	int width, height;

	void init();
	void bind(GLenum unit = GL_FRAMEBUFFER) const;
	void unbind(GLenum unit = GL_FRAMEBUFFER) const;
	void createAttachment(GLint internalFormat, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR);
	void createDepthAttachment(GLint internalFormat, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR);
	void createDepthBuffer();
	const Texture& getAttachment(unsigned int idx) const;
	const Texture& getDepthAttachment() const;
};