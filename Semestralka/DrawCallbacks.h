#pragma once
#include "Material.h"

class DrawCallbacks {
public:
	virtual void supplyMaterial(const Material& material) const {}
	virtual void enableDiffuseTexture(bool value) const {}
	virtual void enableSpecularTexture(bool value) const {}
	virtual void enableNormalMap(bool value) const {}
	virtual void enableDistortion(bool value) const {}
	virtual void enableHeightMap(bool value) const {}
};