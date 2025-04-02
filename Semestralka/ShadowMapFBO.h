#pragma once

#include "pgr.h"
#include "Texture.h"

class ShadowMapFBO {
private:
	GLuint mFbo;
	Texture mShadowMap;
public:
	int width, height;
	ShadowMapFBO();
	void init();
	void bindForWriting();
	void bindForReading();
};