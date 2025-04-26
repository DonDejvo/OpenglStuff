#include "Spritesheet.h"

SpriteRegion Spritesheet::getRegion(unsigned int idx)
{
    SpriteRegion region;
    region.tex = tex;

    int coordX = idx % cols;
    int coordY = idx / cols;

    float x = (float)(margin + (spriteWidth + spacing) * coordX) / tex->getWidth();
    float y = (float)(margin + (spriteHeight + spacing) * coordY) / tex->getHeight();

    region.x = x;
    region.y = y;
    region.w = (float)spriteWidth / tex->getWidth();
    region.h = (float)spriteHeight / tex->getHeight();

    return region;
}
