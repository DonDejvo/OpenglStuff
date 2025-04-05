#pragma once

#include "pgr.h"

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