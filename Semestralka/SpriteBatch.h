#pragma once

#include "pgr.h"
#include "Camera.h"
#include "Material.h"
#include "Geometry.h"
#include "DrawCallbacks.h"
#include "Drawable.h"
#include "SpriteRegion.h"

constexpr unsigned int MAX_SPRITES = 10000;

const glm::vec3 spritePositions[] = {
	{-0.5f, 0.5f, 0.0f},
	{0.5f, 0.5f, 0.0f},
	{0.5f, -0.5f, 0.0f},
	{-0.5f, -0.5f, 0.0f}
};

const glm::vec3 upNormal = glm::vec3(0.0f, 1.0f, 0.0f);

const unsigned int spriteIndices[] = { 0, 2, 1, 0, 3, 2 };

struct Sprite {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	float yaw = 0.0f, pitch = 0.0f;
	Material* material;
	SpriteRegion region;
	bool useFakeNormals = true;
};

class SpriteBatch : public Drawable {
private:
	Geometry mGeometry;
	std::vector<Sprite> mSprites;
	std::vector<Material*> mMaterials;
	glm::mat4 mMatrix = glm::mat4(1.0f);
	int mSpriteCount = 0;
public:
	void init();
	void clear();
	void add(const Sprite& sprite);
	void prepare();
	void sort(Camera* camera);
	void draw(DrawCallbacks* drawCallbacks) const override;

	inline const glm::mat4& getMatrix() const {
		return mMatrix;
	}
	
};