#include "ScreenTechnique.h"

void ScreenTechnique::init()
{
	ShaderTechnique::init();

	glGenVertexArrays(1, &mScreenVAO);
	glBindVertexArray(mScreenVAO);

	float vertices[] = {
		-1.0f,  1.0f,	0.0f, 1.0f,
		 1.0f,  1.0f,	1.0f, 1.0f,
		 1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f, -1.0f,	0.0f, 0.0f
	};

	glGenBuffers(1, &mScreenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mScreenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void ScreenTechnique::blit() {
	glBindVertexArray(mScreenVAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}
