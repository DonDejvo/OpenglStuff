#pragma once

#include "ShaderTechnique.h"

class ScreenTechnique : public ShaderTechnique {
private:
	GLuint mScreenVAO, mScreenVBO;
public:
	void init() override;
	void blit();
};