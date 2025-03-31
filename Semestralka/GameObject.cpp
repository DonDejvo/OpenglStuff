#include "GameObject.h"

GameObject::GameObject(Mesh* mesh, Camera* camera) :
	mMesh(mesh), mCamera(camera)
{
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	pitch = 0;
	yaw = 0;
}

void GameObject::draw(ShaderTechnique& technique)
{
	glm::mat4 PVMMatrix = mCamera->getPVMatrix() * mMatrix;

	technique.supplyPVMMatrix(PVMMatrix);
	technique.supplyModelMatrix(mMatrix);

	mMesh->draw();
}

void GameObject::computeModelMatrix()
{
	mMatrix = glm::mat4(1.0f);
	mMatrix = glm::scale(mMatrix, scale);
	mMatrix = glm::rotate(mMatrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	mMatrix = glm::rotate(mMatrix, yaw, glm::vec3(0.0f, cos(pitch), sin(pitch)));
	mMatrix = glm::translate(mMatrix, position);
}

void GameObject::update(float dt)
{
	computeModelMatrix();
}
