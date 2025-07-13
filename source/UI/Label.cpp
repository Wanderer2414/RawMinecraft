#include "Label.h"
namespace MyBase {

    Label::Label() {}
    Label::~Label() {}
    bool Label::contains(const glm::vec2& position) const {
        return false;
    }
    glm::vec2 Label::getPosition() const {
        return Text::getPosition();
    }
    glm::vec2 Label::getSize() const {
        return Text::getSize();
    }
    void Label::glDraw() const {
        Text::draw();
    }
}