#include "SpriteBatch.h"

void SpriteBatch::init()
{
	mGeometry.init();
}

void SpriteBatch::clear()
{
	mSprites.clear();
}

void SpriteBatch::add(const Sprite& sprite)
{
	mSprites.push_back(sprite);
}

void SpriteBatch::prepare()
{
	mGeometry.vertices.clear();
	mGeometry.indices.clear();
	mGeometry.drawCalls.clear();
	mMaterials.clear();

	unsigned int idx = 0;

	Material* currentMaterial = nullptr;
	unsigned int materialIdx = 0;
	unsigned int offset = 0;

	for (auto& sprite : mSprites) {
		if (idx == 0) {
			currentMaterial = sprite.material;
		}
		else if(currentMaterial != sprite.material) {
			mGeometry.drawCalls.push_back({ 0, offset * 6, (idx - offset) * 6, materialIdx });
			mMaterials.push_back(currentMaterial);

			currentMaterial = sprite.material;
			++materialIdx;
			offset = idx;
		}

		glm::vec2 UVCache[] = {
			{sprite.region[0], sprite.region[1]},
			{sprite.region[0] + sprite.region[2], sprite.region[1]},
			{sprite.region[0] + sprite.region[2], sprite.region[1] + sprite.region[3]},
			{sprite.region[0], sprite.region[1] + sprite.region[3]},
		};

		for (unsigned int i = 0; i < 4; ++i) {
			Geometry::Vertex v;

			glm::vec3 position = glm::vec3(
				spritePositions[i].x * sprite.scale.x, 
				spritePositions[i].y * sprite.scale.y, 
				spritePositions[i].z);

			glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), sprite.yaw, glm::vec3(0, 1, 0));
			position = glm::vec3(rotY * glm::vec4(position, 1.0f));

			position += sprite.position;

			v.position = position;
			v.texCoord = UVCache[i];
			v.normal = spriteNormal;

			mGeometry.vertices.push_back(v);
		}

		for (unsigned int i = 0; i < 6; ++i) {
			mGeometry.indices.push_back(spriteIndices[i] + 4 * idx);
		}

		++idx;
	}

	if (mSprites.size() > 0) {
		mGeometry.drawCalls.push_back({ 0, offset * 6, (idx - offset) * 6, materialIdx });
		mMaterials.push_back(currentMaterial);

		mGeometry.initBuffers();
	}
}

void SpriteBatch::sort(Camera* camera)
{
	auto comp = [camera](const Sprite& sprite1, const Sprite& sprite2) {
		return glm::distance(camera->position, sprite1.position) > glm::distance(camera->position, sprite2.position);
	};
	std::sort(mSprites.begin(), mSprites.end(), comp);
}

void SpriteBatch::draw(DrawCallbacks* drawCallbacks) const
{
	glBindVertexArray(mGeometry.getVAO());

	for (unsigned int i = 0; i < mGeometry.drawCalls.size(); ++i) {
		unsigned int matIdx = mGeometry.drawCalls[i].materialIndex;
		Material* material = matIdx < mMaterials.size() ?
			mMaterials[matIdx] : nullptr;

		if (drawCallbacks && material != nullptr) {
			drawCallbacks->supplyMaterial(*material);

			if (material->diffuseTexture) {
				material->diffuseTexture->bind(DIFFUSE);
				drawCallbacks->enableDiffuseTexture(true);
			}
			else {
				drawCallbacks->enableDiffuseTexture(false);
			}
		}

		mGeometry.draw(i);
	}

	glBindVertexArray(0);
}
