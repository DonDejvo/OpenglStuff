#pragma once

#include "Shader.h"
#include "DrawCallbacks.h"
#include "Drawable.h"
#include "Camera.h"

class ShaderTechnique : public DrawCallbacks {
protected:
	Shader* mShader;
	std::map<unsigned int, GLuint> mTextureLoc;

public:
	void setShader(Shader* shader);
	void use() const;
	virtual void init();
	virtual void draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const;
	void bindTextureUnits() const;
};