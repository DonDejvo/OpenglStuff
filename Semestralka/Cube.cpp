#include "Cube.h"

void Cube::add(SpriteBatch& batch)
{
	Sprite face;
	face.scale.x = scale.x;
	face.scale.y = scale.y;
	face.position = position + glm::vec3(0, 0, scale.z * 0.5f);
	face.material = material;
	face.yaw = 0;
	batch.add(face);
	Sprite face2;
	face2.scale.x = scale.x;
	face2.scale.y = scale.y;
	face2.position = position - glm::vec3(0, 0, scale.z * 0.5f);
	face2.material = material;
	face2.yaw = AI_MATH_PI;
	batch.add(face2);
	Sprite face3;
	face3.scale.x = scale.z;
	face3.scale.y = scale.y;
	face3.position = position + glm::vec3(scale.x * 0.5f, 0, 0);
	face3.material = material;
	face3.yaw = AI_MATH_HALF_PI;
	batch.add(face3);
	Sprite face4;
	face4.scale.x = scale.z;
	face4.scale.y = scale.y;
	face4.position = position - glm::vec3(scale.x * 0.5f, 0, 0);
	face4.material = material;
	face4.yaw = AI_MATH_HALF_PI * 3;
	batch.add(face4);
	Sprite face5;
	face5.scale.x = scale.x;
	face5.scale.y = scale.z;
	face5.position = position + glm::vec3(0, scale.y * 0.5f, 0);
	face5.material = material;
	face5.pitch = AI_MATH_HALF_PI;
	batch.add(face5);
	Sprite face6;
	face6.scale.x = scale.x;
	face6.scale.y = scale.z;
	face6.position = position - glm::vec3(0, scale.y * 0.5f, 0);
	face6.material = material;
	face6.pitch = AI_MATH_HALF_PI * 3;
	batch.add(face6);
}
