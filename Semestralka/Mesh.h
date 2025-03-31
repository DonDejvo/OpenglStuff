#pragma once

#include "pgr.h"
#include <vector>
#include "Texture.h"
#include "Material.h"

constexpr size_t NUM_BUFFERS = 5;
constexpr unsigned int MESH_IMPORT_FLAGS = aiProcess_Triangulate | aiProcess_FlipUVs;

class Mesh {
private:
	enum BufferType {
		VERTEX = 0,
		INDEX = 1
	};

	GLuint mVAO;
	GLuint mBuffers[NUM_BUFFERS];

	void initFromScene(const aiScene* scene, const std::string& path);
	void initSingleMesh(aiMesh* mesh, unsigned int index, int& numVertices, int& numIndices);
	void initMaterial(aiMaterial* material, unsigned int index, const std::string& directory);
protected:
	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	
	struct DrawCall {
		int baseVertex;
		int baseIndex;
		int numIndices;
		int materialIndex;
	};

	std::vector<DrawCall> mDrawCalls;
	std::vector<Material> mMaterials;

public:
	void loadFromFile(const std::string& path);
	void init();
	void draw() const;
};

class Quad : public Mesh {
public:
	Quad();
};