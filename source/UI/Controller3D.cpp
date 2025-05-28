#include "Controller3D.h"
#include "Global.h"
#include "Ray.h"
namespace MyBase3D {

    Controller3D::Controller3D() {
        pIsDoubleClick = pIsFocus = pIsPressed = pIsHovered = pIsReleased = false;
        double_click_count = 0;
    }
    Controller3D::~Controller3D() {
    
    }
    bool Controller3D::isDoubleClick() const {
        return pIsDoubleClick;
    }
    bool Controller3D::isHovered() const {
        return pIsHovered;
    }
    bool Controller3D::isReleased() const {
        return pIsReleased;
    }
    bool Controller3D::isPressed() const {
        return pIsPressed;
    }
    bool Controller3D::isFocus()   const {
        return pIsFocus;
    }
    void Controller3D::setFocus(const bool& focus) {
        pIsFocus = focus;
    }
    bool Controller3D::setHover(const bool& hover) {
        if (pIsHovered!=hover) {
            pIsHovered = hover;
            return true;    
        }
        return false;
    }
    bool Controller3D::setHover(const Ray3f& sight) {
        return setHover(contains(sight));
    }
    void Controller3D::reset() {
        pIsDoubleClick = pIsPressed = pIsReleased = false;
        if (double_click_count>0) double_click_count--;
    }
    _catch_function(Controller3D, BeforeCatch) {
        return false;
    }
    _catch_function(Controller3D, CatchEvent) {
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Button::Left) {
            pIsFocus = pIsPressed = pIsHovered;
        }
        else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Button::Left) {
            pIsReleased = pIsHovered;
            if (pIsReleased) {
                if (double_click_count) pIsDoubleClick = true;
                else double_click_count = 20000;
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
    void Controller3D::draw(RenderTarget& target, RenderStates state) const {
    }
    
    void Controller3D::update() {
        
    }
    void Controller3D::glDraw() const {
    }
}