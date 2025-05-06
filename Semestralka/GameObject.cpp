#include "GameObject.h"

GameObject::GameObject()
{
	game = nullptr;
	name = generateName();
	mMesh = new Mesh();
}

GameObject::~GameObject()
{
	delete mMesh;
}

GameObject::GameObject(const std::string& name)
{
	game = nullptr;
	this->name = name;
	mMesh = new Mesh();
}

void GameObject::init()
{
}

void GameObject::update(float dt)
{
	mMesh->computeModelMatrix();
}

unsigned int GameObject::ids = 0;

void GameObject::collide(GameObject* other)
{
	if (!other->collidable) return;

	float inverseMass1 = getInverseMass();
	float inverseMass2 = other->getInverseMass();

	if (inverseMass1 + inverseMass2 == 0.0f) return;

	glm::vec2 dirVec = glm::vec2(other->getMesh()->position.x, other->getMesh()->position.z) - glm::vec2(getMesh()->position.x, getMesh()->position.z);
	float distance = glm::length(dirVec);
	if (distance < radius + other->radius) {
		glm::vec2 diffVec = glm::normalize(dirVec) * (radius + other->radius - distance);
		getMesh()->position -= glm::vec3(diffVec.x, 0, diffVec.y) * inverseMass1 / (inverseMass1 + inverseMass2);
		other->getMesh()->position += glm::vec3(diffVec.x, 0, diffVec.y) * inverseMass2 / (inverseMass1 + inverseMass2);
	}
}

float GameObject::getInverseMass()
{
	return mass == 0.0f ? 0.0f : 1.0f / mass;
}

std::string GameObject::generateName()
{
	return "__entity__" + std::to_string(ids);
}
