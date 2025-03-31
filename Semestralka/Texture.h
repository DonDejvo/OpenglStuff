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
	void loadFromFile(const std::string& path);
};