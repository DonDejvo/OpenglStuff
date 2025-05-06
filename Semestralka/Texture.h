#pragma once

#include "pgr.h"

constexpr GLenum DIFFUSE = GL_TEXTURE0;
constexpr GLenum SPECULAR = GL_TEXTURE1;
constexpr GLenum SHADOW_MAP = GL_TEXTURE2;
constexpr GLenum DIFFUSE_RED = GL_TEXTURE3;
constexpr GLenum DIFFUSE_GREEN = GL_TEXTURE4;
constexpr GLenum DIFFUSE_BLUE = GL_TEXTURE5;
constexpr GLenum BLEND_MAP = GL_TEXTURE6;
constexpr GLenum HEIGHT_MAP = GL_TEXTURE6;
constexpr GLenum NORMAL_MAP = GL_TEXTURE7;
constexpr GLenum NORMAL_MAP_RED = GL_TEXTURE8;
constexpr GLenum NORMAL_MAP_GREEN = GL_TEXTURE9;
constexpr GLenum NORMAL_MAP_BLUE = GL_TEXTURE10;
constexpr GLenum REFLECTION = GL_TEXTURE0;
constexpr GLenum REFRACTION = GL_TEXTURE1;
constexpr GLenum DISTORTION_MAP = GL_TEXTURE2;
constexpr GLenum CUBE_MAP_SHADOW_MAP = GL_TEXTURE11;

constexpr GLenum TEST_TEX = GL_TEXTURE7;

class Texture {
private:
	GLuint mTextureID;
	std::string mPath;
	int mImageWidth;
	int mImageHeight;
	GLenum mInternalFormat;
public:
	Texture();
	void bind(GLenum texUnit) const;
	void create(int width, int height, GLint internalFormat, GLenum format, GLenum type, GLenum minFilter = GL_LINEAR, GLenum magFilter = GL_LINEAR);
	void loadFromFile(const std::string& path);

	inline int getWidth() const {
		return mImageWidth;
	}

	inline int getHeight() const {
		return mImageHeight;
	}

	inline GLuint getTexID() const {
		return mTextureID;
	}

	inline const std::string& getPath() const {
		return mPath;
	}

	static GLenum getFormat(GLint internalFormat);
};