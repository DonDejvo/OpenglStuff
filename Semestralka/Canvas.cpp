#include "Canvas.h"
#include "Window.h"

void Canvas::init()
{
	mProjMLoc = glGetUniformLocation(shader->getProgramID(), "u_ProjM");

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(CANVAS_NUM_BUFFERS, mBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[BufferType::VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, MAX_QUADS * 4 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	std::vector<unsigned int> indices;
	unsigned int indexCache[] = { 0, 2, 1, 0, 3, 2 };
	for (unsigned int i = 0; i < MAX_QUADS; ++i) {
		for (unsigned int j = 0; j < 6; ++j) {
			indices.push_back(i * 4 + indexCache[j]);
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[BufferType::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Canvas::pushTranslate(float x, float y)
{
	glm::vec2 vec = { x, y };
	translate += vec;
	translateStack.push(vec);
}

void Canvas::popTranslate()
{
	glm::vec2 vec = translateStack.top();
	translate -= vec;
	translateStack.pop();
}

void Canvas::resetTranslate()
{
	translate = { 0, 0 };
	while (!translateStack.empty()) {
		translateStack.pop();
	}
}

void Canvas::beginFrame()
{
	mQuads.clear();
	mDrawCalls.clear();
}

void Canvas::endFrame()
{

	mVerties.resize(mQuads.size() * 4);

	unsigned int offset = 0;
	const Texture* curTex = nullptr;

	for (unsigned int i = 0; i < mQuads.size(); ++i) {
		const Quad& quad = mQuads[i];

		glm::vec2 positions[4] {
			{ quad.x, quad.y },
			{ quad.x + quad.w, quad.y },
			{ quad.x + quad.w, quad.y + quad.h },
			{ quad.x, quad.y + quad.h }
		};

		glm::vec2 uvs[4]{
			{ quad.sx, quad.sy },
			{ quad.sx + quad.sw, quad.sy },
			{ quad.sx + quad.sw, quad.sy + quad.sh },
			{ quad.sx, quad.sy + quad.sh }
		};

		for (unsigned int j = 0; j < 4; ++j) {
			Vertex v;
			v.position = positions[j];
			v.uv = uvs[j];
			v.color = quad.color;
			mVerties[i * 4 + j] = v;
		}

		if (i != 0 && curTex != quad.tex) {
			mDrawCalls.push_back({ offset * 6, (i - offset) * 6, curTex });
			offset = i;

		}
		curTex = quad.tex;
	}

	if (mQuads.size() > 0) {
		mDrawCalls.push_back({ offset * 6, (mQuads.size() - offset) * 6, curTex});

		glBindBuffer(GL_ARRAY_BUFFER, mBuffers[BufferType::VERTEX]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mVerties.size() * sizeof(Vertex), &mVerties[0]);
	}
}

void Canvas::draw()
{
	Window* win = Window::get();

	glViewport(0, 0, win->getWinWdth(), win->getWinHeight());

	GLboolean cullFace = glIsEnabled(GL_CULL_FACE);
	GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
	GLboolean blend = glIsEnabled(GL_BLEND);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->use();

	projM = glm::orthoRH(0.0f, (float)win->getWinWdth(), (float)win->getWinHeight(), 0.0f, -100.0f, 100.0f);
	glUniformMatrix4fv(mProjMLoc, 1, GL_FALSE, &projM[0][0]);

	glBindVertexArray(mVAO);

	for (const DrawCall& drawCall : mDrawCalls) {
		drawCall.tex->bind(GL_TEXTURE0);
		glDrawElements(GL_TRIANGLES, drawCall.count, GL_UNSIGNED_INT, (void*)(drawCall.offset * sizeof(unsigned int)));
	}

	glBindVertexArray(0);

	if (depthTest) {
		glEnable(GL_DEPTH_TEST);
	}
	if (!blend) {
		glDisable(GL_BLEND);
	}
	if (cullFace) {
		glEnable(GL_CULL_FACE);
	}
}

void Canvas::rect(float x, float y, float w, float h)
{
	drawQuad(defaultTexture, 0, 0, 1, 1, x, y, w, h, color);
}

void Canvas::image(const Texture* tex, float sx, float sy, float sw, float sh, float x, float y, float w, float h, const glm::vec4& color)
{
	drawQuad(tex, sx, sy, sw, sh, x, y, w, h, color);
}

void Canvas::image(const Texture* tex, float sx, float sy, float sw, float sh, float x, float y, float w, float h)
{
	image(tex, sx, sy, sw, sh, x, y, w, h, WHITE_COLOR);
}

void Canvas::image(const Texture* tex, float x, float y, float w, float h)
{
	image(tex, 0, 0, 1, 1, x, y, w, h);
}

void Canvas::text(const std::string& text, float x, float y)
{
	const Spritesheet& spritesheet = bitmapFont.spritesheet;
	float charW = fontSize * spritesheet.spriteWidth / spritesheet.spriteHeight;
	float charH = fontSize;
	float offsetX = 0;

	float alignOffsetX = 0;
	switch (textAlign) {
	case TextAlign::CENTER:
		alignOffsetX = charW * text.length() * 0.5f;
		break;
	case TextAlign::RIGHT:
		alignOffsetX = charW * text.length();
		break;
	}

	for (unsigned int i = 0; i < text.length(); ++i) {
		char ch = text[i];

		SpriteRegion region = bitmapFont.getCharRegion(ch);
		image(region.tex, region.x, region.y, region.w, region.h, x + offsetX + alignOffsetX, y, charW, charH, color);
		offsetX += charW;
	}
}

float Canvas::computeTextWidth(const std::string& text)
{
	const Spritesheet& spritesheet = bitmapFont.spritesheet;
	return text.length() * fontSize * spritesheet.spriteWidth / spritesheet.spriteHeight;
}

void Canvas::drawQuad(const Texture* tex, float sx, float sy, float sw, float sh, float x, float y, float w, float h, const glm::vec4& color)
{
	mQuads.push_back({ tex, sx, sy, sw, sh, translate.x + x, translate.y + y, w, h, color });
}
