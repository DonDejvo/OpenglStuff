#pragma once

#include "ShaderTechnique.h"

class ShadowMapTechnique : public ShaderTechnique {
private:
	GLuint mPVMMatrixLoc;
public:
	void init() override;
};