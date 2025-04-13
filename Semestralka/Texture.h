#pragma once

#include "pgr.h"

constexpr GLenum DIFFUSE = GL_TEXTURE0;
constexpr GLenum SPECULAR = GL_TEXTURE1;
constexpr GLenum SHADOW_MAP = GL_TEXTURE2;
constexpr GLenum DIFFUSE_RED = GL_TEXTURE3;
constexpr GLenum DIFFUSE_GREEN = GL_TEXTURE4;
constexpr GLenum DIFFUSE_BLUE = GL_TEXTURE5;
constexpr GLenum BLEND_MAP = GL_TEXTURE6;
constexpr GLenum NORMAL_MAP = GL_TEXTURE7;
constexpr GLenum NORMAL_MAP_RED = GL_TEXTURE8;
constexpr GLenum NORMAL_MAP_GREEN = GL_TEXTURE9;
constexpr GLenum NORMAL_MAP_BLUE = GL_TEXTURE10;

class Texture {
private:
	GLuint mTextureID;
	std::string mPath;
	int mImageWidth;
	int mImageHeight;
public:
	struct CreateParams {
		GLenum format = GL_RGBA;
		GLenum type = GL_UNSIGNED_BYTE;
		GLint filter = GL_LINEAR;
		GLint wrap = GL_CLAMP_TO_EDGE;
	};

	Texture();
	void bind(GLenum texUnit) const;
	void create(int width, int height, const CreateParams& params);
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
};