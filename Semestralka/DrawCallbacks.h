#pragma once
#include "Material.h"

class DrawCallbacks {
public:
	virtual void supplyMaterial(const Material& material) = 0;
	virtual void enableSpecularTexture(bool value) = 0;
};