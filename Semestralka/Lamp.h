#pragma once

#include "pgr.h"
#include "Lights.h"
#include "GameObject.h"

class Lamp : public GameObject {
public:
	PointLight* light;

	~Lamp();

	void init() override;
	void update(float dt) override;
};