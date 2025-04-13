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

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[BufferType::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Geometry::draw(unsigned int idx) const
{
	glDrawElementsBaseVertex(GL_TRIANGLES, drawCalls[idx].numIndices, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * drawCalls[idx].baseIndex), drawCalls[idx].baseVertex);
}

void Geometry::computeTangents()
{
	for (auto& v : vertices) {
		v.tangent = glm::vec3(0.0f);
		v.bitangent = glm::vec3(0.0f);
	}

	for (unsigned int i = 0; i < indices.size(); i += 3) {
		unsigned int idx1 = indices[i];
		unsigned int idx2 = indices[i + 1];
		unsigned int idx3 = indices[i + 2];

		computeTriangleTangent(vertices[idx1], vertices[idx2], vertices[idx3]);
	}

	for (auto& v : vertices) {
		v.tangent = glm::normalize(v.tangent);
		v.bitangent = glm::normalize(v.bitangent);
	}
}

void Geometry::computeTriangleTangent(Vertex& v1, Vertex& v2, Vertex& v3)
{
	glm::vec3 vec1 = v1.position - v3.position;
	glm::vec3 vec2 = v2.position - v3.position;

	glm::vec2 deltaUV1 = v1.texCoord - v3.texCoord;
	glm::vec2 deltaUV2 = v2.texCoord - v3.texCoord;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 tangent;
	tangent.x = f * (deltaUV2.y * vec1.x - deltaUV1.y * vec2.x);
	tangent.y = f * (deltaUV2.y * vec1.y - deltaUV1.y * vec2.y);
	tangent.z = f * (deltaUV2.y * vec1.z - deltaUV1.y * vec2.z);
	tangent = glm::normalize(tangent);

	glm::vec3 bitangent;
	bitangent.x = f * (-deltaUV2.x * vec1.x + deltaUV1.x * vec2.x);
	bitangent.y = f * (-deltaUV2.x * vec1.y + deltaUV1.x * vec2.y);
	bitangent.z = f * (-deltaUV2.x * vec1.z + deltaUV1.x * vec2.z);
	bitangent = glm::normalize(bitangent);

	v1.tangent += tangent;
	v2.tangent += tangent;
	v3.tangent += tangent;

	v1.bitangent += bitangent;
	v2.bitangent += bitangent;
	v3.bitangent += bitangent;
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
	indices = { 0, 2, 1, 0, 3, 2 };
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
				j + i * (widthSegments + 1), j + 1 + (i + 1) * (widthSegments + 1), j + 1 + i * (widthSegments + 1),
				j + 1 + (i + 1) * (widthSegments + 1), j + i * (widthSegments + 1), j + (i + 1) * (widthSegments + 1)
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
	unsigned int indexCache[] = {0, 2, 1, 0, 3, 2};
	for (unsigned int i = 0; i < 6; ++i) {
		for (unsigned int j = 0; j < 6; ++j) {
			indices.push_back(i * 4 + indexCache[j]);
		}
	}
	drawCalls = { { 0, 0, 36, 0 } };
}
