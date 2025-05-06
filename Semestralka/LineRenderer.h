#pragma once
#include "Shader.h"
#include <array>

constexpr unsigned int MAX_LINES = 10000;

class LineRenderer {
private:
	struct Vertex {
		glm::vec3 position;
		glm::vec4 color;
	};

	GLuint mProjViewLoc;

	GLuint mVAO;
	GLuint mVBO;
	unsigned int lineCount;
	Vertex* vertices;
public:
	LineRenderer();
	Shader* shader;

	void init();
	void addLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
	void addRect(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, float yaw, float pitch);
	void addCircle(const glm::vec3& position, float radius, const glm::vec4& color, float yaw, float pitch, int segments);
	//void addBox(const glm::vec3& position, const glm::vec3& scale, const glm::vec4& color, float yaw, float pitch);
	void beginFrame();
	void endFrame();
	void draw(const glm::mat4& PV);
};