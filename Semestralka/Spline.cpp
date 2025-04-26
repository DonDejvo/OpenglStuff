#include "Spline.h"

void Spline::evalCurveSegment(Segment& segment, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
    float t0 = 0.0f;
    float t1 = t0 + pow(distance(p0, p1), alpha);
    float t2 = t1 + pow(distance(p1, p2), alpha);
    float t3 = t2 + pow(distance(p2, p3), alpha);

    glm::vec3 m1 = (1.0f - tension) * (t2 - t1) *
        ((p1 - p0) / (t1 - t0) - (p2 - p0) / (t2 - t0) + (p2 - p1) / (t2 - t1));
    glm::vec3 m2 = (1.0f - tension) * (t2 - t1) *
        ((p2 - p1) / (t2 - t1) - (p3 - p1) / (t3 - t1) + (p3 - p2) / (t3 - t2));

    segment.a = 2.0f * (p1 - p2) + m1 + m2;
    segment.b = -3.0f * (p1 - p2) - m1 - m1 - m2;
    segment.c = m1;
    segment.d = p1;
}

void Spline::recalculateSegments()
{
    segments.clear();

    for (unsigned int i = 0; i < points.size(); ++i) {
        Segment segment;
        unsigned int i0 = i > 0 ? i - 1 : points.size() - 1;
        unsigned int i1 = i;
        unsigned int i2 = (i + 1) % points.size();
        unsigned int i3 = (i + 2) % points.size();
        evalCurveSegment(segment, points[i0], points[i1], points[i2], points[i3]);
        segments.push_back(segment);
    }
}

glm::vec3 Spline::getPoint(float t)
{
    unsigned int idx = (unsigned int)floor(t);
    idx %= segments.size();
    float u = t - floor(t);

    Segment& segment = segments[idx];

    return segment.a * u * u * u +
        segment.b * u * u +
        segment.c * u +
        segment.d;
}

glm::vec3 Spline::getPoint1stDerivate(float t)
{
    unsigned int idx = (unsigned int)floor(t);
    idx %= segments.size();
    float u = t - floor(t);

    Segment& segment = segments[idx];

    return segment.a * 3.0f * u * u +
        segment.b * 2.0f * u +
        segment.c;
}
