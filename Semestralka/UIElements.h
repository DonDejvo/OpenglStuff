#pragma once
#include "Canvas.h"

class UIElements {
private:
	Canvas* mCanvas;
	bool mIsNextButtonHovered;

	glm::vec2 getTranslatedMousePos();
	bool popNextButtonHovered();
public:
	float fontSize;
	glm::vec4 textColor, bgColor, hoverTextColor, hoverBgColor;
	glm::vec2 padding;
	float lineHeight;

	UIElements();
	void setCanvas(Canvas* c);
	inline Canvas* getCanvas() const {
		return mCanvas;
	}

	void hoverNextButton();
	void text(const std::string& text, float x, float y);
	bool button(const std::string& text, float x, float y, float w, float h);
	bool button(const std::string& text, float x, float y);
	bool imageButton(const Texture* tex, float sx, float sy, float sw, float sh, float x, float y, float w, float h);
	void begin(float x, float y, float w, float h);
	void end();

};