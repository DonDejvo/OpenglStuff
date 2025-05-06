#pragma once

#include "ShaderTechnique.h"

class SimpleTechnique : public ShaderTechnique {
protected:
	GLuint PVMLocation;
	GLuint mDiffuseColorLoc;
	GLuint mColorLoc;
	GLuint uDiffuseEnabledLoc;
public:
	void init() override;
	void supplyPVMMatrix(const glm::mat4& PVMMatrix) const;
	void draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const override;
	void supplyMaterial(const Material& material) const override;
	void enableDiffuseTexture(bool value) const override;
};