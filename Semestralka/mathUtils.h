#pragma once

#include "pgr.h"

template <typename T>
T step(T a, T b, T t) {
	return a + (b - a) * t;
}

template <typename T>
T clamp(T x, T min, T max) {
	return std::min(std::max(x, min), max);
}

float calcBarrycentricCoordHeight(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float x, float z);

float random();

glm::vec3 HSLtoRGB(const glm::vec3& hsl);