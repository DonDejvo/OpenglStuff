#pragma once

#include "pgr.h"
#include "Geometry.h"
#include "Drawable.h"
#include "DrawCallbacks.h"
#include "Material.h"

class WaterTile : public Drawable {
private:
	QuadGeometry mGeometry;
	glm::mat4 mMatrix;
	WaterMaterrial* mMaterial;
public:
	glm::vec3 position;
	float width, height;

	WaterTile();

	void init();
	void computeModelMatrix();
	void draw(DrawCallbacks* drawCallbacks) const override;

	inline const glm::mat4& getMatrix() const {
		return mMatrix;
	}

	void setMaterial(WaterMaterrial* mat);

	inline virtual const glm::vec3& getColor() const override {
		return glm::vec3(1.0f);
	}
};