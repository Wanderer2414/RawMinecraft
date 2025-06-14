#include "Controller.h"
#include "Global.h"
namespace MyBase {

    Controller::Controller() {
        __isDoubleClick = __isFocus = __isPressed = __isHovered = __isReleased = false;
        __doubleClickCount = 0;
    }
    Controller::~Controller() {
    
    }
    bool Controller::isDoubleClick() const {
        return __isDoubleClick;
    }
    bool Controller::isHovered() const {
        return __isHovered;
    }
    bool Controller::isReleased() const {
        return __isReleased;
    }
    bool Controller::isPressed() const {
        return __isPressed;
    }
    bool Controller::isFocus()   const {
        return __isFocus;
    }
    void Controller::setFocus(const bool& focus) {
        __isFocus = focus;
    }
    bool Controller::setHover(const bool& hover) {
        if (__isHovered!=hover) {
            __isHovered = hover;
            return true;    
        }
        return false;
    }
    bool Controller::setHover(const glm::vec2& position) {
        return setHover(contains(position));
    }
    bool Controller::catchEvent(GLFWwindow* window) {
        return false;
    }
    bool Controller::sensitiveHandle(GLFWwindow* window) {
        return false;
    }
    void Controller::reset() {
        __isDoubleClick = __isPressed = __isReleased = false;
        if (__doubleClickCount>0) __doubleClickCount--;
    }
    bool Controller::handle(GLFWwindow* window) {
        // if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
        //     __isFocus = __isPressed = __isHovered;
        // }
        // else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left) {
        //     __isReleased = __isHovered;
        //     if (__isReleased) {
        //         if (__doubleClickCount) __isDoubleClick = true;
        //         else __doubleClickCount = 20000;
        //     }
        // }
        return false;
    }
    bool Controller::contains(const glm::vec2& position) const {
        return false;
    }
    glm::vec2 Controller::getPosition() const {
        return {0, 0};
    }
    glm::vec2 Controller::getSize() const {
        return {0, 0};
    }
    void Controller::glDraw() const {

    }
    void Controller::setPosition(const float& x, const float& y) {
        
    }
    
    void Controller::update() {
        
    }
    
}