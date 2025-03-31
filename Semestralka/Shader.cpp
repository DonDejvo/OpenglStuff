#include "Shader.h"

#include <sstream>
#include <regex>
#include <iostream>

void Shader::loadFromString(const char* vertexSource, const char* fragmentSource)
{
	// Create program
    GLuint shaders[] = {
        pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexSource),
        pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentSource),
        0
    };

    mProgramID = pgr::createProgram(shaders);
}

void Shader::loadFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    // Create program
    GLuint shaders[] = {
        pgr::createShaderFromFile(GL_VERTEX_SHADER, vertexShaderPath),
        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderPath),
        0
    };

    mProgramID = pgr::createProgram(shaders);
}

void Shader::use() const
{
	glUseProgram(mProgramID);
}
