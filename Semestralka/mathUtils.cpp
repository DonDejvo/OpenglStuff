#include "mathUtils.h"

float calcBarrycentricCoordHeight(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float x, float z)
{
	float area = abs((p1.x - p3.x) * (p2.z - p3.z) - (p2.x - p3.x) * (p1.z - p3.z));
	float a = abs((p1.x - x) * (p2.z - z) - (p2.x - x) * (p1.z - z)) / area;
	float b = abs((p1.x - x) * (p3.z - z) - (p3.x - x) * (p1.z - z)) / area;
	float c = 1.0f - a - b;
	return p3.y * a + p2.y * b + p1.y * c;
}
