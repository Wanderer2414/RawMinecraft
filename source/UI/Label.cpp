#include "Label.h"
namespace MyBase {

    Label::Label(const Font& font):__font(font) {}
    Label::~Label() {}
    void Label::setText(const std::string& text) {
        __text = text;
    }
    void Label::setPosition(const float& x, const float& y) {
        __position = {x, y};
    }
    void Label::setColor(const glm::vec3& color) {
        __color = color;
    }
    void Label::glDraw() const {
        __font.DrawText(__text, __position, __color);
    }
}