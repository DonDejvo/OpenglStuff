#include "BitmapFont.h"

SpriteRegion BitmapFont::getCharRegion(char ch)
{
    for (unsigned int i = 0; i < charset.size(); ++i) {
        if (ch == charset[i]) {
            return spritesheet.getRegion(i);
        }
    }
    return SpriteRegion();
}
