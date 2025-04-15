#pragma once

#include "FogTechnique.h"
#include "Framebuffer.h"
#include "WaterTile.h"

class WaterTechnique : public FogTechnique {
private:
	Framebuffer mReflectionFramebuffer;
	Framebuffer mRefractionFramebuffer;

	GLuint modelLocation;

	GLuint mCameraPosLoc;

	GLuint mDistortionEnabledLoc;
	GLuint mDistortionIntensity;
	GLuint mTimeLoc;
public:
	void init() override;

	inline Framebuffer& getReflectionFramebuffer() {
		return mReflectionFramebuffer;
	}

	inline Framebuffer& getRefractionFramebuffer() {
		return mRefractionFramebuffer;
	}

	void bindReflectionFBO(WaterTile& tile, Camera& camera) const;
	void unbindReflectionFBO(WaterTile& tile, Camera& camera) const;
	void bindRefractionFBO(WaterTile& tile, Camera& camera) const;
	void unbindRefractionFBO(WaterTile& tile, Camera& camera) const;

	virtual void draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const;

	void supplyModelMatrix(const glm::mat4& modelMatrix) const;
	void supplyCameraPosition(const glm::vec3& pos) const;
	void supplyMaterial(const Material& material) const override;
	void enableDistortion(bool value) const override;
	void supplyTime(float time) const;
};