#pragma once

#include "SimpleTechnique.h"
#include "Framebuffer.h"

class WaterTechnique : public SimpleTechnique {
private:
	Framebuffer mReflectionFramebuffer;
	Framebuffer mRefractionFramebuffer;
public:
	void init() override;

	inline Framebuffer& getReflectionFramebuffer() {
		return mReflectionFramebuffer;
	}

	inline Framebuffer& getRefractionFramebuffer() {
		return mRefractionFramebuffer;
	}
};