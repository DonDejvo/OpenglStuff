#include "Texture.h"
#include <iostream>

Texture::Texture()
{
	mInternalFormat = 0;
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

void Texture::create(int width, int height, GLint internalFormat, GLenum format, GLenum type, GLenum minFilter, GLenum magFilter)
{
	mInternalFormat = internalFormat;
	mImageWidth = width;
	mImageHeight = height;

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::loadFromFile(const std::string& path)
{
	mPath = path;

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// Load the image
	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (!ilLoadImage((wchar_t*)path.c_str())) {
		std::cout << "Error loading image: " << path << std::endl;
		return;
	}

	// Convert image to RGB format if it's not
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	// Get image dimensions
	mImageWidth = ilGetInteger(IL_IMAGE_WIDTH);
	mImageHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	mInternalFormat = GL_RGBA;

	// Access pixel data
	ILubyte* data = ilGetData();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mImageWidth, mImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);               // Magnification doesn't use mipmapping

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	ilDeleteImages(1, &imageID);
}

GLenum Texture::getFormat(GLint internalFormat)
{
	switch (internalFormat) {
	case GL_RGBA8:
		return GL_RGBA;
	case GL_RGB8:
		return GL_RGB;
	case GL_DEPTH_COMPONENT16:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT32:
		return GL_DEPTH_COMPONENT;
	case GL_RGB8I:
	case GL_RGB8UI:
	case GL_RGB16I:
	case GL_RGB16UI:
	case GL_RGB32I:
	case GL_RGB32UI:
		return GL_RGB_INTEGER;
	case GL_R32F:
		return GL_RED;
	}
}
