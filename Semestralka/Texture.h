#pragma once

#include "pgr.h"

class Texture {
private:
	GLuint mTextureID;
	std::string mPath;
	int mImageWidth;
	int mImageHeight;
public:
	Texture();
	void bind(GLenum texUnit) const;
	void create(int width, int height);
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