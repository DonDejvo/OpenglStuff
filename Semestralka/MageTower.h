#pragma once

#include "GameObject.h"
#include "Lights.h"

class MageTower : public GameObject {
private:
	SpotLight* light;
	float angle = 0.0f;
public:
	~MageTower();
	void init() override;
	void update(float dt) override;
};