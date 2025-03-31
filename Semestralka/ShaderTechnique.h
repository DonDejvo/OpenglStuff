#pragma once

#include "Shader.h"

class ShaderTechnique {
protected:
	GLuint PVMLocation;
	GLuint modelLocation;
	Shader* mShader;

	virtual void init();
public:
	ShaderTechnique(Shader* shader);
	void use() const;
	void supplyPVMMatrix(const glm::mat4& PVMMatrix);
	void supplyModelMatrix(const glm::mat4& modelMatrix);
};