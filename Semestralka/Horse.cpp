#include "Horse.h"
#include "Game.h"

void Horse::init()
{
	spline = ((Game*)game)->spline;

	mMesh->loadFromFile("data/horse/CABALLO_low_poly.obj");
	mMesh->scale *= 0.75f;
	mMesh->offset.y = 3.1f;
	mMesh->offsetYaw = -AI_MATH_HALF_PI;
	radius = 2.0f;

    initialPosition = mMesh->position;
}

void Horse::update(float dt)
{
	GameObject::update(dt);

	t += dt;

	mMesh->position = initialPosition + spline->getPoint(t);
	glm::vec3 direction = spline->getPoint1stDerivate(t);
	mMesh->yaw = atan2(-direction.z, direction.x) + mMesh->offsetYaw;

	mMesh->position.y = ((Game*)game)->terrainManager->getHeightAtPosition(mMesh->position);
}
