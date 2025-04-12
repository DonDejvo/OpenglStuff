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

void Texture::create(int width, int height, const CreateParams& params)
{
	mImageWidth = width;
	mImageHeight = height;

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, params.format, width, height, 0, params.format, params.type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrap);

	glBindTexture(GL_TEXTURE_2D, 0);
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
