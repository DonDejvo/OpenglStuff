#pragma once
#include "DrawCallbacks.h"

class Drawable {
public:
	virtual void draw(DrawCallbacks* drawCallbacks) const = 0;
	virtual const glm::mat4& getMatrix() const = 0;
	virtual unsigned int getID() const {
		return 0;
	};
	virtual const glm::vec3& getColor() const = 0;
};