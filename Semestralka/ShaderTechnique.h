#pragma once

#include "Shader.h"
#include "DrawCallbacks.h"

class ShaderTechnique : public DrawCallbacks {
protected:
	Shader* mShader;

public:
	void setShader(Shader* shader);
	void use() const;
	virtual void init() = 0;
};