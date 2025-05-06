#pragma once

#include "pgr.h"
#include <vector>
#include "Texture.h"
#include "Material.h"
#include "DrawCallbacks.h"
#include "Geometry.h"
#include "Drawable.h"

constexpr unsigned int MESH_IMPORT_FLAGS = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;

class Mesh : public Drawable {
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
	static unsigned int ids;
	static Data loadDataFromFile(const std::string& path);

	unsigned int id;
	glm::vec3 position;
	glm::vec3 scale;
	float pitch, yaw;
	glm::vec3 color;
	glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);
	float offsetYaw = 0.0f;

	Mesh();

	void loadFromFile(const std::string& path);
	void draw(DrawCallbacks* drawCallbacks) const override;

	inline void setGeometry(Geometry* geom) {
		mData.geometry = geom;
	}

	void setMaterial(unsigned int idx, Material* mat);

	inline const glm::mat4& getMatrix() const override {
		return mMatrix;
	}

	void computeModelMatrix();

	virtual void update(float dt);

	unsigned int getID() const override;

	inline std::vector<Material*> getMaterials() {
		return mData.materials;
	}

	inline virtual const glm::vec3& getColor() const override {
		return color;
	}
};