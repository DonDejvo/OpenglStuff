#pragma once

#include "ShaderTechnique.h"
#include "Framebuffer.h"

class ShadowMapTechnique : public ShaderTechnique {
private:
	GLuint mPVMMatrixLoc;
	Framebuffer mShadowMapFBO;
public:
	void init() override;
	void supplyPVMMatrix(const glm::mat4& matrix) const;
	void bindFBO() const;
	void unbindFBO() const;
	void bindShadowMap();
	void draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const override;
};