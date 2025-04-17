#include "Grass.h"

void Grass::place(SpriteBatch& spriteBatch)
{
	Sprite sprite;
	sprite.scale *= size;
	sprite.material = material;
	sprite.position = position;
	sprite.yaw = rand() % 100 / 100.0f * AI_MATH_HALF_PI;

	sprite.region[0] = regionX;
	sprite.region[1] = regionY;
	sprite.region[2] = regionWidth;
	sprite.region[3] = regionHeight;

	Sprite sprite2;
	sprite2.scale *= size;
	sprite2.material = material;
	sprite2.position = position;
	sprite2.yaw = sprite.yaw + AI_MATH_HALF_PI;

	sprite2.region[0] = regionX;
	sprite2.region[1] = regionY;
	sprite2.region[2] = regionWidth;
	sprite2.region[3] = regionHeight;

	spriteBatch.add(sprite);
	spriteBatch.add(sprite2);
}
