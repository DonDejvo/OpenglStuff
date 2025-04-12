#pragma once

#include "pgr.h"
#include <vector>
#include <map>

class Shader {
private:
	GLuint mProgramID = 0;
public:
	void loadFromString(const char* vertexSource, const char* fragmentSource);
	void loadFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void use() const;
	inline GLuint getProgramID() const {
		return mProgramID;
	}
};