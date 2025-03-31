#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "ShaderTechnique.h"

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
	void draw(ShaderTechnique& technique);
	void computeModelMatrix();
	virtual void update(float dt);
};