#pragma once
#include "pgr.h"
#include "Texture.h"

class Framebuffer {
private:
	GLuint mFBO;
	std::vector<Texture> mColorAttachments;
	Texture mDepthAttachment;
	GLuint mRBO;
public:
	int width, height;

	void init();
	void bind() const;
	void unbind() const;
	void createAttachment(GLenum internalFormat, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR);
	void createDepthAttachment(GLenum internalFormat, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR);
	void createDepthBuffer();
	const Texture& getAttachment(unsigned int idx) const;
	const Texture& getDepthAttachment() const;
};