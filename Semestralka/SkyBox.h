#pragma once

#include <vector>
#include <string>
#include "pgr.h"

class SkyBox {
private:
	GLuint mTexID;
public:
	/**
	(right, left, top, bottom, front, back)
	*/
	void loadFromFiles(const std::vector<std::string>& paths);
	void bind(GLenum texUnit) const;
};