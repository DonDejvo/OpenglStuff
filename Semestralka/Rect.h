#pragma once

#include "pgr.h"

class Rect {
public:
	float x, y, w, h;

	Rect(float x, float y, float w, float h);
	bool intersects(const Rect& rect);
	bool contains(const glm::vec2& point);
};