#pragma once

#include "pgr.h"

class Spline {
private:
	struct Segment {
		glm::vec3 a, b, c, d;
	};
	std::vector<Segment> segments;
public:
	std::vector<glm::vec3> points;
	float alpha = 0.5f;
	float tension = 0.0f;

	void evalCurveSegment(Segment& segment, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
	void recalculateSegments();
	glm::vec3 getPoint(float t);
	glm::vec3 getPoint1stDerivate(float t);
};