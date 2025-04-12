#include "Geometry.h"
#include "mathUtils.h"

void Geometry::init()
{
}

void Geometry::initBuffers() {
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(NUM_BUFFERS, mBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[BufferType::VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[BufferType::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Geometry::draw(unsigned int idx) const
{
	glDrawElementsBaseVertex(GL_TRIANGLES, drawCalls[idx].numIndices, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * drawCalls[idx].baseIndex), drawCalls[idx].baseVertex);
}

void QuadGeometry::init() {
	float halfScaleX = scale.x * 0.5f;
	float halfScaleY = scale.y * 0.5f;

	vertices = {
		{ {-halfScaleX, halfScaleY, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
		{ {halfScaleX, halfScaleY, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
		{ {halfScaleX, -halfScaleY, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {-halfScaleX, -halfScaleY, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} }
	};
	indices = { 0, 1, 2, 0, 2, 3 };
	drawCalls = {
		{0, 0, 6, 0}
	};
}

void PlaneGeometry::init()
{
	float halfScaleX = scale.x * 0.5f;
	float halfScaleZ = scale.z * 0.5f;
	
	for (int i = 0; i <= heightSegments; ++i) {
		float ty = (float)i / heightSegments;
		float posY = step(-halfScaleZ, halfScaleZ, ty);
		float texCoordY = step(1.0f, 0.0f, ty);

		for (int j = 0; j <= widthSegments; ++j) {
			float tx = (float)j / widthSegments;
			float posX = step(-halfScaleX, halfScaleX, tx);
			float texCoordX = step(0.0f, 1.0f, tx);

			Vertex v;
			v.position = glm::vec3(posX, 0, posY);
			v.texCoord = glm::vec2(texCoordX, texCoordY);
			v.normal = glm::vec3(0.0f, 1.0f, 0.0f);

			vertices.push_back(v);
		}
	}

	for (unsigned int i = 0; i < (unsigned int)heightSegments; ++i) {
		for (unsigned int j = 0; j < (unsigned int)widthSegments; ++j) {
			unsigned int indexCache[] = { 
				j + i * (widthSegments + 1),j + 1 + i * (widthSegments + 1), j + 1 + (i + 1) * (widthSegments + 1),
				j + 1 + (i + 1) * (widthSegments + 1), j + (i + 1) * (widthSegments + 1), j + i * (widthSegments + 1)
			};
			for (unsigned int k = 0; k < 6; ++k) {
				indices.push_back(indexCache[k]);
			}
		}
	}

	drawCalls = { { 0, 0, 6 * widthSegments * heightSegments, 0 } };
}

void PlaneGeometry::recalculateNormals()
{
	for (int i = 0; i <= heightSegments; ++i) {
		for (int j = 0; j <= widthSegments; ++j) {
			float heightL = getHeight(j - 1, i);
			float heightR = getHeight(j + 1, i);
			float heightT = getHeight(j, i - 1);
			float heightB = getHeight(j, i + 1);

			glm::vec3 normal = glm::vec3(heightL - heightR, 2.0f, heightT - heightB);
			normal = glm::normalize(normal);

			vertices[i * (widthSegments + 1) + j].normal = normal;
		}
	}
}

float PlaneGeometry::getHeight(int x, int y) const
{
	x = clamp(x, 0, widthSegments);
	y = clamp(y, 0, heightSegments);
	return vertices[y * (widthSegments + 1) + x].position.y;
}

void CubeGeometry::init()
{
	float halfScaleX = scale.x * 0.5f;
	float halfScaleY = scale.y * 0.5f;
	float halfScaleZ = scale.z * 0.5f;

	glm::vec2 uvCache[] = {
		{0.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f},
		{0.0f, 0.0f}
	};

	vertices = {
		// front
		{ {-halfScaleX, halfScaleY, halfScaleZ}, uvCache[0], {0.0f, 0.0f, 1.0f}},
		{ {halfScaleX, halfScaleY, halfScaleZ}, uvCache[1], {0.0f, 0.0f, 1.0f} },
		{ {halfScaleX, -halfScaleY, halfScaleZ}, uvCache[2], {0.0f, 0.0f, 1.0f} },
		{ {-halfScaleX, -halfScaleY, halfScaleZ}, uvCache[3], {0.0f, 0.0f, 1.0f} },
		// back
		{ {halfScaleX, halfScaleY, -halfScaleZ}, uvCache[0], {0.0f, 0.0f, -1.0f} },
		{ {-halfScaleX, halfScaleY, -halfScaleZ}, uvCache[1], {0.0f, 0.0f, -1.0f} },
		{ {-halfScaleX, -halfScaleY, -halfScaleZ}, uvCache[2], {0.0f, 0.0f, -1.0f} },
		{ {halfScaleX, -halfScaleY, -halfScaleZ}, uvCache[3], {0.0f, 0.0f, -1.0f} },
		// left
		{ {-halfScaleX, halfScaleY, -halfScaleZ}, uvCache[0], {-1.0f, 0.0f, 0.0f} },
		{ {-halfScaleX, halfScaleY, halfScaleZ}, uvCache[1], {-1.0f, 0.0f, 0.0f} },
		{ {-halfScaleX, -halfScaleY, halfScaleZ}, uvCache[2], {-1.0f, 0.0f, 0.0f} },
		{ {-halfScaleX, -halfScaleY, -halfScaleZ}, uvCache[3], {-1.0f, 0.0f, 0.0f} },
		// right
		{ {halfScaleX, halfScaleY, halfScaleZ}, uvCache[0], {1.0f, 0.0f, 0.0f} },
		{ {halfScaleX, halfScaleY, -halfScaleZ}, uvCache[1], {1.0f, 0.0f, 0.0f} },
		{ {halfScaleX, -halfScaleY, -halfScaleZ}, uvCache[2], {1.0f, 0.0f, 0.0f} },
		{ {halfScaleX, -halfScaleY, halfScaleZ}, uvCache[3], {1.0f, 0.0f, 0.0f} },
		// top
		{ {-halfScaleX, halfScaleY, -halfScaleZ}, uvCache[0], {0.0f, 1.0f, 0.0f} },
		{ {halfScaleX, halfScaleY, -halfScaleZ}, uvCache[1], {0.0f, 1.0f, 0.0f} },
		{ {halfScaleX, halfScaleY, halfScaleZ}, uvCache[2], {0.0f, 1.0f, 0.0f} },
		{ {-halfScaleX, halfScaleY, halfScaleZ}, uvCache[3], {0.0f, 1.0f, 0.0f} },
		// bottom
		{ {-halfScaleX, -halfScaleY, halfScaleZ}, uvCache[0], {0.0f, -1.0f, 0.0f} },
		{ {halfScaleX, -halfScaleY, halfScaleZ}, uvCache[1], {0.0f, -1.0f, 0.0f} },
		{ {halfScaleX, -halfScaleY, -halfScaleZ}, uvCache[2], {0.0f, -1.0f, 0.0f} },
		{ {-halfScaleX, -halfScaleY, -halfScaleZ}, uvCache[3], {0.0f, -1.0f, 0.0f} },
	};
	unsigned int indexCache[] = {0, 1, 2, 0, 2, 3};
	for (unsigned int i = 0; i < 6; ++i) {
		for (unsigned int j = 0; j < 6; ++j) {
			indices.push_back(i * 4 + indexCache[j]);
		}
	}
	drawCalls = { { 0, 0, 36, 0 } };
}
