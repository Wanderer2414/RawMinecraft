#include "Controller2D.h"
namespace MyBase {

    Controller2D::Controller2D():
        __isDoubleClick(false), __isFocus(false), __isPressed(false), __isMouseDown(false),
        __isHovered(false), __isReleased(false), __isVisible(true) {
        __clickCount =  0;
    }
    Controller2D::~Controller2D() {
    
    }
    bool Controller2D::isDoubleClick() const {
        return __isDoubleClick;
    }
    bool Controller2D::isHovered() const {
        return __isHovered;
    }
    bool Controller2D::isMouseDown() const {
        return __isMouseDown;
    }
    bool Controller2D::isReleased() const {
        return __isReleased;
    }
    bool Controller2D::isPressed() const {
        return __isPressed;
    }
    bool Controller2D::isFocus()   const {
        return __isFocus;
    }
    bool Controller2D::isVisible() const {
        return __isVisible;
    }
    void Controller2D::setFocus(const bool& focus) {
        __isFocus = focus;
    }
    bool Controller2D::setHover(const bool& hover) {
        if (__isHovered!=hover) {
            __isHovered = hover;
            if (!hover) {
                __isMouseDown = __isPressed = __isDoubleClick = false;
                return __lostHover();
            }
            else return __hover();
        }
        return false;
    }
    bool Controller2D::setHover(const glm::vec2& position) {
        return setHover(contains(position));
    }
    void Controller2D::setVisible(const bool& visible) {
        __isVisible = visible;
    }
    void Controller2D::reset() {
        __isDoubleClick = __isReleased = __isPressed = false;
        if (__clickCount) __clickCount--;
    }
    bool Controller2D::catchEvent(GLFWwindow* window) {
        bool is_changed = false;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (__isHovered) {
                if (!__clickCount) {
                    if (!__isFocus) is_changed = __getFocus() || is_changed;
                    __isFocus = __isPressed = true;
                    is_changed = __clicked() || is_changed;
                }
                else if (__clickCount<90) __isFocus = __isDoubleClick = true;
                __clickCount = 100;
                __isMouseDown = true;
                is_changed = __mouseDown() || is_changed;
            }
            else {
                __isFocus = false;
                is_changed = __lostFocus() || is_changed;
            }
        }
        else if (__isMouseDown) {
            __isReleased = true;
            __isMouseDown = false;
            is_changed = __mouseRelease() || is_changed;
        }
        if (__isFocus) __onFocus();
        return is_changed;
    }
    bool Controller2D::__lostFocus() { return false;};
    bool Controller2D::__getFocus() { return false;};
    bool Controller2D::__onFocus() { return false;};
    bool Controller2D::__clicked() { return false;};
    bool Controller2D::__mouseDown() { return false;};
    bool Controller2D::__mouseRelease() { return false;};
    bool Controller2D::__hover() { return false;};
    bool Controller2D::__lostHover() { return false;};
} ;