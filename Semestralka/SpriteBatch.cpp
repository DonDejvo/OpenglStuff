#include "SpriteBatch.h"

void SpriteBatch::init()
{
	mGeometry.init();

	std::vector<unsigned int> indices;
	unsigned int indexCache[] = { 0, 2, 1, 0, 3, 2 };
	for (unsigned int i = 0; i < MAX_SPRITES; ++i) {
		for (unsigned int j = 0; j < 6; ++j) {
			indices.push_back(i * 4 + indexCache[j]);
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGeometry.mBuffers[Geometry::BufferType::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mGeometry.mBuffers[Geometry::BufferType::VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Geometry::Vertex) * MAX_SPRITES * 4, nullptr, GL_DYNAMIC_DRAW);
}

void SpriteBatch::clear()
{
	mSprites.clear();
	mSpriteCount = 0;
}

void SpriteBatch::add(const Sprite& sprite)
{
	mSprites.push_back(sprite);
	++mSpriteCount;
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

	for (unsigned int i = 0; i < mSpriteCount; ++i) {
		auto& sprite = mSprites[i];

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
			{sprite.region.x, sprite.region.y},
			{sprite.region.x + sprite.region.w, sprite.region.y},
			{sprite.region.x + sprite.region.w, sprite.region.y + sprite.region.h},
			{sprite.region.x, sprite.region.y + sprite.region.h},
		};

		for (unsigned int i = 0; i < 4; ++i) {
			Geometry::Vertex v;

			glm::vec3 position = glm::vec3(
				spritePositions[i].x * sprite.scale.x, 
				spritePositions[i].y * sprite.scale.y, 
				spritePositions[i].z);

			glm::mat4 rotMat = glm::mat4(1.0f);
			rotMat = glm::rotate(rotMat, sprite.pitch, glm::vec3(1.0f, 0.0f, 0.0f));
			rotMat = glm::rotate(rotMat, sprite.yaw, glm::vec3(0.0f, cos(sprite.pitch), sin(sprite.pitch)));
			position = glm::vec3(rotMat * glm::vec4(position, 1.0f));

			position += sprite.position;

			v.position = position;
			v.texCoord = UVCache[i];

			if (sprite.useFakeNormals) {
				v.normal = upNormal;
			}
			else {
				v.normal = glm::mat3(rotMat) * glm::vec3(0.0f, 0.0f, 1.0f);
			}

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

		glBindBuffer(GL_ARRAY_BUFFER, mGeometry.mBuffers[Geometry::BufferType::VERTEX]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mGeometry.vertices.size() * sizeof(Geometry::Vertex), &mGeometry.vertices[0]);
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
