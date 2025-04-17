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
	glm::vec3 scale = glm::vec3(1.0f);

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	struct DrawCall {
		unsigned int baseVertex;
		unsigned int baseIndex;
		unsigned int numIndices;
		unsigned int materialIndex;
	};

	std::vector<DrawCall> drawCalls;

	virtual void init();
	void initBuffers();
	void draw(unsigned int idx) const;
	void computeTangents();

	inline GLuint getVAO() const {
		return mVAO;
	}
protected:
	void computeTriangleTangent(Vertex& v1, Vertex& v2, Vertex& v3);
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
	void recalculateNormals();
	float getHeight(int x, int y) const;
};

class CubeGeometry : public Geometry {
public:
	void init() override;
};