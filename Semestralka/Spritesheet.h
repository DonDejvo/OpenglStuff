#pragma once
#include "Texture.h"
#include "SpriteRegion.h"

class Spritesheet {
public:
	Texture* tex;
	int numSprites;
	int cols;
	int spriteWidth, spriteHeight;
	int spacing, margin;

	SpriteRegion getRegion(unsigned int idx);
};