#pragma once

#include "pgr.h"
#include <vector>
#include "Texture.h"
#include "Material.h"
#include "DrawCallbacks.h"
#include "Geometry.h"

constexpr unsigned int MESH_IMPORT_FLAGS = aiProcess_Triangulate | aiProcess_FlipUVs;

class Mesh {
private:
	void initFromScene(const aiScene* scene, const std::string& path);
	void initSingleMesh(aiMesh* mesh, unsigned int index, int& numVertices, int& numIndices);
	void initMaterial(aiMaterial* material, unsigned int index, const std::string& directory);
protected:
	glm::mat4 mMatrix;
	Geometry* mGeometry;
	std::vector<Material> mMaterials;

public:
	glm::vec3 position;
	glm::vec3 scale;
	float pitch, yaw;

	Mesh();

	void loadFromFile(const std::string& path);
	void draw(DrawCallbacks* drawCallbacks) const;

	inline void setGeometry(Geometry* geom) {
		mGeometry = geom;
	}

	void setMaterial(unsigned int idx, const Material& mat);

	inline const glm::mat4& getMatrix() const {
		return mMatrix;
	}

	void computeModelMatrix();
};