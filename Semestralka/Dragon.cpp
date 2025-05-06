#include "Dragon.h"
#include "mathUtils.h"
#include "ParticleSystem.h"
#include "Game.h"

void Dragon::init()
{
	mMesh->loadFromFile("data/dragon/jadedragon.obj");
	mMesh->scale *= 0.75f;
	mMesh->offset.y = 1.0f;
	radius = 4.0f;
}

void Dragon::update(float dt)
{
	GameObject::update(dt);

	particlesCounter += dt;
	if (particlesCounter > particleSpawnDelay) {
		particlesCounter = 0.0f;

		for (unsigned int i = 0; i < 2; ++i) {
			const glm::vec3& scale = getMesh()->scale;
			float yaw = getMesh()->yaw;
			Particle p(getMesh()->position + glm::vec3(-sin(yaw) * scale.z * 8, scale.y * (5.0f + mMesh->offset.y), -cos(yaw) * scale.z * 8), 2, 0, 2.5f + random() * 2.5f, ((Game*)game)->particleAnim);
			p.velocity = { -sin(yaw + random() * 0.6f - 0.3f) * particleSpeed, random() * 2 - 1, -cos(yaw + random() * 0.6f - 0.3f) * particleSpeed };
			((Game*)game)->particles.push_back(p);
		}
	}
}
