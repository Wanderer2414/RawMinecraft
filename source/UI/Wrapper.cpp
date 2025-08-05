#include "Wrapper.h"
namespace MyBase {

    Wrapper::Wrapper(Controller2D* controller): __controller(controller) {}
    Wrapper::~Wrapper() {}
    bool Wrapper::isDoubleClick() const {
        return __controller->isDoubleClick();
    }
    bool Wrapper::isHovered() const {
        return __controller->isHovered();
    }
    bool Wrapper::isMouseDown() const {
        return __controller->isMouseDown();
    }
    bool Wrapper::isReleased() const {
        return __controller->isReleased();
    }
    bool Wrapper::isPressed() const {
        return __controller->isPressed();
    }
    bool Wrapper::isFocus()   const {
        return __controller->isFocus();
    }
    bool Wrapper::isVisible() const {
        return __controller->isVisible();
    }
    glm::vec2 Wrapper::getSize() const {
        return __controller->getSize();
    }
    glm::vec2 Wrapper::getPosition() const {
        return __controller->getPosition();
    }
    void Wrapper::setFocus(const bool& focus) {
        __controller->setFocus(focus);
    }
    bool Wrapper::setHover(const bool& hover) {
        return __controller->setHover(hover);
    }
    bool Wrapper::setHover(const glm::vec2& position) {
        return __controller->setHover(contains(position));
    }
    void Wrapper::setVisible(const bool& visible) {
        return __controller->setVisible(visible);
    }
    void Wrapper::reset() {
        __controller->reset();
    }
    void Wrapper::changeState(Controller2D* controller) {
        __controller = controller;
        __controller->update();
    }

    bool Wrapper::contains(const glm::vec2& position) const {
        return __controller->contains(position);
    }
    bool Wrapper::catchEvent(GLFWwindow* window) {
        return __controller->catchEvent(window);
    }
    bool Wrapper::handle(GLFWwindow* window) {
        return __controller->handle(window);
    }
    void Wrapper::glDraw() const {
        return __controller->glDraw();
    }
    void Wrapper::glDrawTransparent() const {
        return __controller->glDrawTransparent();
    }
} ;