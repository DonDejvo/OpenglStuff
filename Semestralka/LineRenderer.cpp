#include "LineRenderer.h"
#include <iostream>

LineRenderer::LineRenderer()
{
	mVAO = 0;
	mVBO = 0;
	shader = nullptr;
	vertices = new Vertex[MAX_LINES * 2];
	lineCount = 0;
	mProjViewLoc = 0;
}

void LineRenderer::init()
{
	mProjViewLoc = glGetUniformLocation(shader->getProgramID(), "u_ProjView");

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_LINES * 2, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glBindVertexArray(0);
}

void LineRenderer::addLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color)
{
	if (lineCount == MAX_LINES) return;

	vertices[lineCount * 2].position = start;
	vertices[lineCount * 2].color = color;

	vertices[lineCount * 2 + 1].position = end;
	vertices[lineCount * 2 + 1].color = color;

	++lineCount;
}

void LineRenderer::addRect(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, float yaw, float pitch)
{
	std::vector<glm::vec3> points = {
		{-scale.x * 0.5f, scale.y * 0.5f, 0},
		{scale.x * 0.5f, scale.y * 0.5f, 0},
		{scale.x * 0.5f, -scale.y * 0.5f, 0},
		{-scale.x * 0.5f, -scale.y * 0.5f, 0}
	};

	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::rotate(matrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, yaw, glm::vec3(0.0f, cos(pitch), sin(pitch)));
	matrix = glm::translate(glm::mat4(1.0f), position) * matrix;

	for (unsigned int i = 0; i < points.size(); ++i) {
		points[i] = glm::vec3(matrix * glm::vec4(points[i], 1.0));
	}

	for (unsigned int i = 0; i < points.size(); ++i) {
		addLine(points[i],points[(i + 1) % points.size()], color);
	}
}

void LineRenderer::addCircle(const glm::vec3& position, float radius, const glm::vec4& color, float yaw, float pitch, int segments)
{
	std::vector<glm::vec3> points;
	for (int i = 0; i < segments; ++i) {
		float angle = (float)i / segments * AI_MATH_TWO_PI;
		points.push_back({ cos(angle) * radius, sin(angle) * radius, 0 });
	}

	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::rotate(matrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, yaw, glm::vec3(0.0f, cos(pitch), sin(pitch)));
	matrix = glm::translate(glm::mat4(1.0f), position) * matrix;

	for (unsigned int i = 0; i < points.size(); ++i) {
		points[i] = glm::vec3(matrix * glm::vec4(points[i], 1.0));
	}

	for (unsigned int i = 0; i < points.size(); ++i) {
		addLine(points[i], points[(i + 1) % points.size()], color);
	}
}

void LineRenderer::beginFrame()
{
	lineCount = 0;
}

void LineRenderer::endFrame()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, lineCount * 2 * sizeof(Vertex), vertices);
}

void LineRenderer::draw(const glm::mat4& PV)
{
	GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);

	glDisable(GL_DEPTH_TEST);

	shader->use();

	glUniformMatrix4fv(mProjViewLoc, 1, GL_FALSE, & PV[0][0]);

	glBindVertexArray(mVAO);

	glDrawArrays(GL_LINES, 0, lineCount * 2);

	glBindVertexArray(0);

	if (depthTest) {
		glEnable(GL_DEPTH_TEST);
	}
}
