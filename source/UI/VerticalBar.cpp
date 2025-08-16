#include "VerticalBar.h"
#include "ControlCenter.h"
#include "Shape.h"
#include "ShapeManager.h"

namespace MyBase {
    VerticalBar::VerticalBar(): __height(0), __maxValues(1), __value(0), __isScrollDown(false) {
        ShapeManager::getInstance().createShape(__rectangle, {width, __height});
        ShapeContainer::setFillColor({200, 200, 200, 255});
        __scrollButton.setNormalColor(WHITE);
        __scrollButton.setHoverColor({100,100,100,255});
        __scrollButton.setClickColor({50,50,50,255});
        __scrollButton.setSize({width*0.9f, 0.2});
        insert(&__scrollButton);
    }
    VerticalBar::~VerticalBar() {
        ShapeManager::getInstance().removeShape(__rectangle, {width, __height});
    }
    bool VerticalBar::contains(const glm::vec2& position) const {
        return __rectangle.contains(position-getPosition());
    }
    bool VerticalBar::catchEvent(GLFWwindow* window) {
        bool is_changed = Container2D::catchEvent(window);
        float y = -ControlCenter::getInstance().getScroll().y/2.f;
        if (y) {
            y*=__maxValues;
            __value += y;
            __value = std::clamp(__value, 0.f, __maxValues);
            update();
            is_changed = true;
        }
        if (__scrollButton.isPressed()) {
            __isScrollDown = true;
            __mouseStartDrag = ControlCenter::getInstance().getCursorPos(window).y - __scrollButton.getPosition().y;
        }
        if (__scrollButton.isReleased()) __isScrollDown = false;
        if (__isScrollDown) {
            glm::vec2 mousePos = ControlCenter::getInstance().getCursorPos(window);
            __value = (1-(mousePos.y - __mouseStartDrag - getPosition().y)/(__height-0.2))*__maxValues;
            __value = std::clamp(__value, 0.f, __maxValues);
            update();
            is_changed = true;
        }
        return is_changed;
    }
    float VerticalBar::getValue() const {
        return __value;
    }
    void VerticalBar::setMaxValue(const float& value) {
        __maxValues = value;
        update();
    }
    void VerticalBar::update() {
        __scrollButton.setPosition({getPosition().x+width*0.05f, getPosition().y+(__height-0.2)*(1-__value/__maxValues)});
    }
    void VerticalBar::setPosition(const glm::vec2& position) {
        glm::vec2 offset = position - getPosition();
        ShapeContainer::setPosition(position);
        update();
    }
    void VerticalBar::setHeight(const float& height) {
        if (height != __height) {
            ShapeManager::getInstance().removeShape(__rectangle, {width, __height});
            __height = height;
            ShapeManager::getInstance().createShape(__rectangle, {width, __height});
        }
    }
    void VerticalBar::glDraw() const {
        ShapeContainer::draw(__rectangle);
        Container2D::glDraw();
    }
    glm::vec2 VerticalBar::getPosition() const {
        return ShapeContainer::getPosition();
    }
    glm::vec2 VerticalBar::getSize() const {
        return {width, __height};
    }
}