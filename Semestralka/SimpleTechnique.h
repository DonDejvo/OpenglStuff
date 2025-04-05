#pragma once

#include "ShaderTechnique.h"

class SimpleTechnique : public ShaderTechnique {
protected:
	GLuint PVMLocation;
public:
	void init() override;
	void supplyPVMMatrix(const glm::mat4& PVMMatrix) const;
};