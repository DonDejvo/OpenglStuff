#pragma once

#include "pgr.h"
#include "Shader.h"
#include "Texture.h"
#include <stack>
#include "BitmapFont.h"
#include "Color.h"

constexpr unsigned int MAX_QUADS = 10000;
constexpr size_t CANVAS_NUM_BUFFERS = 2;

class Canvas {
private:
	enum BufferType {
		VERTEX = 0,
		INDEX = 1
	};
	struct Quad {
		const Texture* tex;
		float sx, sy, sw, sh;
		float x, y, w, h;
		glm::vec4 color;
	};

	struct DrawCall {
		unsigned int offset;
		unsigned int count;
		const Texture* tex;
	};

	struct Vertex {
		glm::vec2 position;
		glm::vec2 uv;
		glm::vec4 color;
	};

	GLuint mVAO;
	GLuint mBuffers[CANVAS_NUM_BUFFERS];
	std::vector<Vertex> mVerties;
	std::vector<DrawCall> mDrawCalls;
	std::vector<Quad> mQuads;

	GLuint mProjMLoc;

	void drawQuad(const Texture* tex, float sx, float sy, float sw, float sh, float x, float y, float w, float h, const glm::vec4& color);
public:
	enum class TextAlign {
		LEFT, RIGHT, CENTER
	};

	Shader* shader;
	Texture* defaultTexture;
	glm::vec4 color = WHITE_COLOR;
	glm::mat4 projM;
	std::stack<glm::vec2> translateStack;
	glm::vec2 translate;

	BitmapFont bitmapFont;
	float fontSize = 32;
	TextAlign textAlign = TextAlign::LEFT;

	void init();
	void pushTranslate(float x, float y);
	void popTranslate();
	void resetTranslate();
	void beginFrame();
	void endFrame();
	void draw();
	
	void rect(float x, float y, float w, float h);
	void image(const Texture* tex, float sx, float sy, float sw, float sh, float x, float y, float w, float h, const glm::vec4& color);
	void image(const Texture* tex, float sx, float sy, float sw, float sh, float x, float y, float w, float h);
	void image(const Texture* tex, float x, float y, float w, float h);
	void text(const std::string& text, float x, float y);
	float computeTextWidth(const std::string& text);
	inline const glm::mat4& getProjMatrix() const {
		return projM;
	}
};