#include "WaterTile.h"

WaterTile::WaterTile()
{
	mMaterial = nullptr;
	width = 10.0f;
	height = 10.0f;
	position = glm::vec3(0.0f);
}

void WaterTile::init()
{
	mGeometry.scale.x = width;
	mGeometry.scale.y = height;

	mGeometry.init();
	mGeometry.initBuffers();
}

void WaterTile::computeModelMatrix()
{
	mMatrix = glm::mat4(1.0f);
	mMatrix = glm::rotate(mMatrix, (float)-AI_MATH_HALF_PI, glm::vec3(1.0f, 0.0f, 0.0f));
	mMatrix = glm::translate(glm::mat4(1.0f), position) * mMatrix;
}

void WaterTile::draw(DrawCallbacks* drawCallbacks) const
{
	glBindVertexArray(mGeometry.getVAO());

	if (drawCallbacks) {
		drawCallbacks->supplyMaterial(*mMaterial);

		if (mMaterial->distortionTexture) {
			mMaterial->distortionTexture->bind(DISTORTION_MAP);
			drawCallbacks->enableDistortion(true);
		}
		else {
			drawCallbacks->enableDistortion(false);
		}
	}

	mGeometry.draw(0);

	glBindVertexArray(0);
}

void WaterTile::setMaterial(WaterMaterrial* mat)
{
	mMaterial = mat;
}
