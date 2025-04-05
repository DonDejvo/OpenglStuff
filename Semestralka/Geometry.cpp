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

void QuadGeometry::init() {
	vertices = {
		{ {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} }
	};
	indices = { 0, 1, 2, 0, 2, 3 };
	drawCalls = {
		{0, 0, 6, 0}
	};
}

void PlaneGeometry::init()
{
	
	for (int i = 0; i <= heightSegments; ++i) {
		float ty = (float)i / heightSegments;
		float posY = step(-0.5f, 0.5f, ty);
		float texCoordY = step(0.0f, 1.0f, ty);

		for (int j = 0; j <= widthSegments; ++j) {
			float tx = (float)j / widthSegments;
			float posX = step(-0.5f, 0.5f, tx);
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

void CubeGeometry::init()
{
	vertices = {
		// front
		{ {-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
		{ {-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
		// back
		{ {0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
		{ {-0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
		{ {-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
		{ {0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
		// left
		{ {-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
		{ {-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
		{ {-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
		// right
		{ {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
		{ {0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} },
		{ {0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f} },
		{ {0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f} },
		// top
		{ {-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
		{ {0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f} },
		{ {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
		{ {-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f} },
		// bottom
		{ {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
		{ {0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
		{ {0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
		{ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
	};
	unsigned int indexCache[] = {0, 1, 2, 0, 2, 3};
	for (unsigned int i = 0; i < 6; ++i) {
		for (unsigned int j = 0; j < 6; ++j) {
			indices.push_back(i * 4 + indexCache[j]);
		}
	}
	drawCalls = { { 0, 0, 36, 0 } };
}
