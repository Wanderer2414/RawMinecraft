#include "Controller2D.h"
#include "ControlCenter.h"
#include "GLFW/glfw3.h"
namespace MyBase {

    Controller2D::Controller2D():
        __isDoubleClick(false), __isFocus(false), __isPressed(false), __isMouseDown(false),
        __isHovered(false), __isReleased(false), __isVisible(true) {
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
                __lostHover();
            }
            else __hover();
            return true;
        }
        else if (hover) return __onHover();
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
    }
    bool Controller2D::catchEvent(GLFWwindow* window) {
        bool is_changed = false;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
            if (__isHovered) {
                if (ControlCenter::getInstance().IsMouseClicked()) {
                    if (!__isFocus) is_changed = __focus(window) || is_changed;
                    __isFocus = __isPressed = true;
                    is_changed = __mouseClicked(window) || is_changed;
                }
                if (ControlCenter::getInstance().IsDoubleClicked()) {
                    __isDoubleClick = true;
                }
                __isMouseDown = true;
                is_changed = __mouseDown(window) || is_changed;
            }
            else {
                __isFocus = false;
                is_changed = __lostFocus(window) || is_changed;
            }
        }
        else if (__isMouseDown) {
            __isReleased = true;
            __isMouseDown = false;
            is_changed = __mouseRelease(window) || is_changed;
        }
        if (__isFocus) is_changed = __onFocus(window) || is_changed;
        return is_changed;
    }
    bool Controller2D::__lostFocus(GLFWwindow*) { return false;};
    bool Controller2D::__focus(GLFWwindow*) { return false;};
    bool Controller2D::__onFocus(GLFWwindow*) { return false;};
    bool Controller2D::__mouseClicked(GLFWwindow*) { return false;};
    bool Controller2D::__mouseDown(GLFWwindow*) { return false;};
    bool Controller2D::__mouseRelease(GLFWwindow*) { return false;};
    bool Controller2D::__hover() { return false;};
    bool Controller2D::__onHover() { return false;};
    bool Controller2D::__lostHover() { return false;};
} ;