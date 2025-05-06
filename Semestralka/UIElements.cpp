#include "UIElements.h"
#include "Input.h"
#include "Rect.h"

glm::vec2 UIElements::getTranslatedMousePos()
{
    glm::vec2& translate = mCanvas->translate;
    return glm::vec2(Input::get()->getMouseX(), Input::get()->getMouseY()) - translate;
}

bool UIElements::popNextButtonHovered()
{
    bool hovered = mIsNextButtonHovered;
    mIsNextButtonHovered = false;
    return hovered;
}

UIElements::UIElements()
{
    mCanvas = nullptr;
    fontSize = 32;
    textColor = GRAY_COLOR;
    bgColor = glm::vec4(0, 0, 0, 0.5f);
    hoverTextColor = WHITE_COLOR;
    hoverBgColor = BLACK_COLOR;
    lineHeight = 36;
    padding = glm::vec2(10, 10);
    mIsNextButtonHovered = false;
}

void UIElements::setCanvas(Canvas* c)
{
    mCanvas = c;
}

void UIElements::hoverNextButton()
{
    mIsNextButtonHovered = true;
}

void UIElements::text(const std::string& text, float x, float y)
{
    mCanvas->fontSize = fontSize;
    mCanvas->color = textColor;
    mCanvas->text(text, x, y);
}

bool UIElements::button(const std::string& text, float x, float y, float w, float h)
{
    glm::vec2 pos = getTranslatedMousePos();
    mCanvas->fontSize = fontSize;
    Rect buttonRect(x, y, w, h);
    bool hovered = buttonRect.contains(pos);
    bool clicked = hovered && Input::get()->isMouseButtonClicked(GLUT_LEFT_BUTTON);

    mCanvas->pushTranslate(x, y);
    bool shouldHover = popNextButtonHovered();
    mCanvas->color = hovered || shouldHover ? hoverBgColor : bgColor;
    mCanvas->rect(0, 0, w, h);
    mCanvas->color = hovered || shouldHover ? hoverTextColor : textColor;
    mCanvas->text(text, padding.x, padding.y);
    mCanvas->popTranslate();

    return clicked;
}

bool UIElements::button(const std::string& text, float x, float y)
{
    mCanvas->fontSize = fontSize;
    float w = mCanvas->computeTextWidth(text) + padding.x * 2;
    float h = fontSize + 2 * padding.y;
    return button(text, x, y, w, h);
}

bool UIElements::imageButton(const Texture* tex, float sx, float sy, float sw, float sh, float x, float y, float w, float h)
{
    glm::vec2 pos = getTranslatedMousePos();
    Rect buttonRect(x, y, w, h);
    bool hovered = buttonRect.contains(pos);
    bool clicked = hovered && Input::get()->isMouseButtonClicked(GLUT_LEFT_BUTTON);

    mCanvas->pushTranslate(x, y);
    bool shouldHover = popNextButtonHovered();
    mCanvas->color = hovered || shouldHover ? hoverBgColor : bgColor;
    mCanvas->rect(0, 0, w, h);
    mCanvas->image(tex, sx, sy, sw, sh, padding.x, padding.y, w - padding.x * 2, h - padding.y * 2);
    mCanvas->popTranslate();

    return clicked;
}

void UIElements::begin(float x, float y, float w, float h)
{
    mCanvas->pushTranslate(x, y);
    mCanvas->color = bgColor;
    mCanvas->rect(0, 0, w, h);
    mCanvas->pushTranslate(padding.x, padding.y);
}

void UIElements::end()
{
    mCanvas->popTranslate();
    mCanvas->popTranslate();
}
