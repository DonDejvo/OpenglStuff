#pragma once

#include "pgr.h"
#include <vector>
#include "Texture.h"
#include "Material.h"
#include "DrawCallbacks.h"
#include "Geometry.h"

constexpr unsigned int MESH_IMPORT_FLAGS = aiProcess_Triangulate | aiProcess_FlipUVs;

class Mesh {
public:
	struct Data {
		Geometry* geometry;
		std::vector<Material*> materials;
	};
private:
	static void initFromScene(Data& data, const aiScene* scene, const std::string& path);
	static void initSingleMesh(Data& data, aiMesh* mesh, unsigned int index, int& numVertices, int& numIndices);
	static void initMaterial(Data& data, aiMaterial* material, unsigned int index, const std::string& directory);
protected:
	glm::mat4 mMatrix;
	Data mData;
public:
	static Data loadDataFromFile(const std::string& path);

	glm::vec3 position;
	glm::vec3 scale;
	float pitch, yaw;

	Mesh();

	void loadFromFile(const std::string& path);
	void draw(DrawCallbacks* drawCallbacks) const;

	inline void setGeometry(Geometry* geom) {
		mData.geometry = geom;
	}

	void setMaterial(unsigned int idx, Material* mat);

	inline const glm::mat4& getMatrix() const {
		return mMatrix;
	}

	void computeModelMatrix();
};