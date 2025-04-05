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
	void bind() const;
	void unbind() const;

	inline Texture& getShadowMap() {
		return mShadowMap;
	}
};