#include "Grass.h"

void Grass::place(SpriteBatch& spriteBatch)
{
	Sprite sprite;
	sprite.scale *= size;
	sprite.material = material;
	sprite.position = position;
	sprite.yaw = rand() % 100 / 100.0f * AI_MATH_HALF_PI;

	sprite.region.x = regionX;
	sprite.region.y = regionY;
	sprite.region.w = regionWidth;
	sprite.region.h = regionHeight;

	Sprite sprite2;
	sprite2.scale *= size;
	sprite2.material = material;
	sprite2.position = position;
	sprite2.yaw = sprite.yaw + AI_MATH_HALF_PI;

	sprite2.region.x = regionX;
	sprite2.region.y = regionY;
	sprite2.region.w = regionWidth;
	sprite2.region.h = regionHeight;

	spriteBatch.add(sprite);
	spriteBatch.add(sprite2);
}
