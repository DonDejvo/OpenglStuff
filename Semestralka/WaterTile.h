#pragma once

#include "pgr.h"
#include "Geometry.h"

class WaterTile {
private:
	QuadGeometry mGeometry;
	glm::mat4 mMatrix;
public:
	glm::vec3 position;
	float width, height;

	WaterTile();

	void init();
	void computeModelMatrix();
	void draw() const;

	inline const glm::mat4& getMatrix() const {
		return mMatrix;
	}
};