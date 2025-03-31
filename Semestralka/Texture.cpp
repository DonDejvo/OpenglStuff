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

void Texture::loadFromFile(const std::string& path)
{
	mTextureID = pgr::createTexture(path);

	mPath = path;

	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &mImageWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &mImageHeight);

	glBindTexture(GL_TEXTURE_2D, 0);
}
