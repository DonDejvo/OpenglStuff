#pragma once

#include "ShaderTechnique.h"
#include "CubemapFramebuffer.h"

class CubemapShadowMapTechnique : public ShaderTechnique {
private:
	GLuint mPVMMatrixLoc;
	GLuint mModelMatrixLoc;
	GLuint mLightPosLoc;
	CubemapFramebuffer mCubemapShadowMapFBO;
public:
	struct CameraDirection {
		GLenum cubemapUnit;
		glm::vec3 direction;
		glm::vec3 up;
	};

	CameraDirection cameraDirections[6] = {
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
		{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
		{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f, 0.0f } },
	};

	void init() override;
	void supplyPVMMatrix(const glm::mat4& matrix) const;
	void supplyModelMatrix(const glm::mat4& matrix) const;
	void supplyLightPos(const glm::vec3& pos) const;

	void bindCubemapFBO(GLenum cubemapUnit) const;
	void unbindCubemapFBO() const;
	void bindCubemapShadowMap();

	void draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const override;
};