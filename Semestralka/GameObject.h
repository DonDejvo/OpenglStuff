#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"

class GameObject {
private:
	Mesh* mMesh;
	Camera* mCamera;
	glm::mat4 mMatrix;
public:
	glm::vec3 position;
	glm::vec3 scale;
	float pitch, yaw;

	GameObject(Mesh* mesh, Camera* camera);
	void computeModelMatrix();
	virtual void update(float dt);

	inline const Mesh& getMesh() const {
		return *mMesh;
	}

	inline const Camera& getCamera() const {
		return *mCamera;
	}

	inline const glm::mat4& getMatrix() const {
		return mMatrix;
	}
};