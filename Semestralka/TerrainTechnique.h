#pragma once

#include "LightingTechnique.h"

class TerrainTechnique : public LightingTechnique {
protected:
	GLuint mHeightMapEnabledLoc;
public:
	void init() override;
	void enableHeightMap(bool value) const override;
};