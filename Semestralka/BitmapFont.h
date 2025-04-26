#pragma once
#include "Spritesheet.h"

class BitmapFont {
public:
	Spritesheet spritesheet;
	std::string charset;

	SpriteRegion getCharRegion(char ch);
};