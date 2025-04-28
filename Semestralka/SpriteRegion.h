#pragma once

class SpriteRegion {
public:
	Texture* tex;
	float x, y, w, h;

	SpriteRegion() {
		tex = nullptr;
		x = 0;
		y = 0;
		w = 1;
		h = 1;
	}
};