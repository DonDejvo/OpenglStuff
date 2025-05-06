#include "mathUtils.h"

float calcBarrycentricCoordHeight(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float x, float z)
{
	float area = abs((p1.x - p3.x) * (p2.z - p3.z) - (p2.x - p3.x) * (p1.z - p3.z));
	float a = abs((p1.x - x) * (p2.z - z) - (p2.x - x) * (p1.z - z)) / area;
	float b = abs((p1.x - x) * (p3.z - z) - (p3.x - x) * (p1.z - z)) / area;
	float c = 1.0f - a - b;
	return p3.y * a + p2.y * b + p1.y * c;
}

float random()
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return r;
}

glm::vec3 HSLtoRGB(const glm::vec3& hsl)
{
    float h = hsl.x;
    float s = hsl.y;
    float l = hsl.z;

    auto hue2rgb = [](float p, float q, float t) {
        if (t < 0.0f) t += 1.0f;
        if (t > 1.0f) t -= 1.0f;
        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
        if (t < 1.0f / 2.0f) return q;
        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
        return p;
        };

    float r, g, b;

    if (s == 0.0f) {
        r = g = b = l;
    }
    else {
        float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;
        r = hue2rgb(p, q, h + 1.0f / 3.0f);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1.0f / 3.0f);
    }

    return glm::vec3(r, g, b);
}