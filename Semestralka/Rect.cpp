#include "Rect.h"

Rect::Rect(float x, float y, float w, float h)
: x(x), y(y), w(w), h(h)
{
}

bool Rect::intersects(const Rect& rect)
{
	return (x + w - rect.x) * (x - (rect.x + rect.w)) < 0 &&
		(y + h - rect.y) * (y - (rect.y + rect.h)) < 0;
}

bool Rect::contains(const glm::vec2& point)
{
	return point.x > x && point.x < x + w &&
		point.y > y && point.y < y + h;
}
