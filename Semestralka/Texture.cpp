#include "Texture.h"

Texture::Texture()
{
	mTextureID = 0;
	mImageWidth = 0;
	mImageHeight = 0;
	mPath = "";
}

void Texture::bind(GLenum texUnit) const
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::create(int width, int height, GLenum internalFormat, GLenum type, GLenum minFilter, GLenum magFilter)
{
	mInternalFormat = internalFormat;

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::loadFromFile(const std::string& path)
{
	mTextureID = pgr::createTexture(path);

	mPath = path;

	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &mImageWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &mImageHeight);

	glBindTexture(GL_TEXTURE_2D, 0);
}
