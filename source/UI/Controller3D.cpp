#include "Controller3D.h"
#include "Global.h"
#include "Ray.h"
namespace MyBase3D {

    Controller3D::Controller3D() {
        __isDoubleClick = __isFocus = __isPressed = __isHovered = __isReleased = false;
        __doubleClickCount = 0;
    }
    Controller3D::~Controller3D() {
    
    }
    bool Controller3D::isDoubleClick() const {
        return __isDoubleClick;
    }
    bool Controller3D::isHovered() const {
        return __isHovered;
    }
    bool Controller3D::isReleased() const {
        return __isReleased;
    }
    bool Controller3D::isPressed() const {
        return __isPressed;
    }
    bool Controller3D::isFocus()   const {
        return __isFocus;
    }
    void Controller3D::setFocus(const bool& focus) {
        __isFocus = focus;
    }
    bool Controller3D::setHover(const bool& hover) {
        if (__isHovered!=hover) {
            __isHovered = hover;
            return true;    
        }
        return false;
    }
    bool Controller3D::setHover(const Ray3f& sight) {
        return setHover(contains(sight));
    }
    void Controller3D::reset() {
        __isDoubleClick = __isPressed = __isReleased = false;
        if (__doubleClickCount>0) __doubleClickCount--;
    }
    _catch_function(Controller3D, BeforeCatch) {
        return false;
    }
    _catch_function(Controller3D, CatchEvent) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
            __isFocus = __isPressed = __isHovered;
        }
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left) {
            __isReleased = __isHovered;
            if (__isReleased) {
                if (__doubleClickCount) __isDoubleClick = true;
                else __doubleClickCount = 20000;
            }
        }
        return false;
    }
    _catch_function(Controller3D, AfterCatch) {
        return false;
    }
    _handle_function(Controller3D, handle) {
        return false;
    }
    bool Controller3D::contains(const Ray3f& position) const {
        return false;
    }
    void Controller3D::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    }
    
    void Controller3D::update() {
        
    }
    void Controller3D::glDraw() const {
    }
}