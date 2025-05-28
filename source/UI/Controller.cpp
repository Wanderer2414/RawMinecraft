#include "Controller.h"
#include "Global.h"
namespace MyBase {

    Controller::Controller() {
        m_is_double_click = m_is_focus = m_is_pressed = m_is_hovered = m_is_released = false;
        double_click_count = 0;
    }
    Controller::~Controller() {
    
    }
    bool Controller::isDoubleClick() const {
        return m_is_double_click;
    }
    bool Controller::isHovered() const {
        return m_is_hovered;
    }
    bool Controller::isReleased() const {
        return m_is_released;
    }
    bool Controller::isPressed() const {
        return m_is_pressed;
    }
    bool Controller::isFocus()   const {
        return m_is_focus;
    }
    void Controller::setFocus(const bool& focus) {
        m_is_focus = focus;
    }
    bool Controller::setHover(const bool& hover) {
        if (m_is_hovered!=hover) {
            m_is_hovered = hover;
            return true;    
        }
        return false;
    }
    bool Controller::setHover(const Vector2f& position) {
        return setHover(contains(position));
    }
    void Controller::reset() {
        m_is_double_click = m_is_pressed = m_is_released = false;
        if (double_click_count>0) double_click_count--;
    }
    _catch_function(Controller, BeforeCatch) {
        return false;
    }
    _catch_function(Controller, CatchEvent) {
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Button::Left) {
            m_is_focus = m_is_pressed = m_is_hovered;
        }
        else if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Button::Left) {
            m_is_released = m_is_hovered;
            if (m_is_released) {
                if (double_click_count) m_is_double_click = true;
                else double_click_count = 20000;
            }
        }
        return false;
    }
    _catch_function(Controller, AfterCatch) {
        return false;
    }
    _handle_function(Controller, handle) {
        return false;
    }
    bool Controller::contains(const Vector2f& position) const {
        return false;
    }
    Vector2f Controller::getPosition() const {
        return {0, 0};
    }
    Vector2f Controller::getSize() const {
        return {0, 0};
    }
    void Controller::draw(RenderTarget& target, RenderStates state) const {
    }
    void Controller::setPosition(const float& x, const float& y) {
        
    }
    
    void Controller::update() {
        
    }
    
}