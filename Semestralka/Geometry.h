#pragma once

#include "pgr.h"

constexpr size_t NUM_BUFFERS = 5;

class Geometry {
private:
	enum BufferType {
		VERTEX = 0,
		INDEX = 1
	};

	GLuint mVAO;
	GLuint mBuffers[NUM_BUFFERS];
public:
	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	struct DrawCall {
		int baseVertex;
		int baseIndex;
		int numIndices;
		int materialIndex;
	};

	std::vector<DrawCall> drawCalls;

	virtual void init();
	void initBuffers();

	inline GLuint getVAO() const {
		return mVAO;
	}
};

class QuadGeometry : public Geometry {
public:
	void init() override;
};

class PlaneGeometry : public Geometry {
public:
	int widthSegments = 8;
	int heightSegments = 8;
	void init() override;
};

class CubeGeometry : public Geometry {
public:
	void init() override;
};