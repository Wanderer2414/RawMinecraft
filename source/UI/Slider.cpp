#include "Slider.h"
#include "ControlCenter.h"
#include "Shape.h"
#include "Global.h"
#include "Rectangle.h"
#include "ShapeManager.h"

namespace MyBase  {

    Slider::Slider(): width(1.0), maxVal(100), minVal(0), value(50), isScroll(false) {
        ShapeManager::getInstance().createShape(__rectangle, {width, height});
        ShapeContainer::setFillColor({200, 200, 200, 255});
        __scrollButton.setNormalColor(WHITE);
        __scrollButton.setHoverColor({100,100,100,255});
        __scrollButton.setClickColor({50,50,50,255});
        __scrollButton.setSize({0.2, height*0.9});
        insert(&__scrollButton);
        Text::setText("50");
        Text::setScale({0.05, 0.05});
        Text::setTextColor(BLACK);
        update();

    }
    Slider::~Slider(){
        ShapeManager::getInstance().removeShape(__rectangle, {width, height});
    }
    void Slider::setPosition(const glm::vec2& position){
        glm::vec2 offset = position - getPosition();
        ShapeContainer::setPosition(position);
        update();
    }
    void Slider::setWidth(const float& width){
        if(this->width != width){
            ShapeManager::getInstance().removeShape(__rectangle, {width, height});
            this->width = width;
            ShapeManager::getInstance().createShape(__rectangle, {width, height});
        }
    }
    void Slider::setMaxValue(const int & value){
        if(value > minVal){
            maxVal = value;
        }
    }
    void Slider::setMinValue(const int & value){
        if(value < maxVal){
            minVal = value;
        }
    }
    void Slider::setText(const std::string& text){
        Text::setText(text);
        Text::setPosition({getPosition().x + width/2 - Text::getSize().x/2, getPosition().y - height});
    }
    glm::vec2 Slider::getPosition() const {
        return ShapeContainer::getPosition();
    }
    glm::vec2 Slider::getSize() const {
        return {width, height};
    }
    int Slider::getValue() const{
        return value;
    }
    bool Slider::catchEvent(GLFWwindow * window) {
        bool is_changed = Container2D::catchEvent(window);
        float x = ControlCenter::getInstance().getScroll().x/2.f;
        if(x){
            x *= (maxVal - minVal)/10.f;
            value += x;
            value = std::clamp(value, minVal, maxVal);
            update();
            is_changed = true;
        }
        if(__scrollButton.isPressed()){
            isScroll = true;
            mouseStartDrag = ControlCenter::getInstance().getCursorPos(window).x - __scrollButton.getPosition().x;
        }
        if(__scrollButton.isReleased()) isScroll = false;
        if(isScroll){
            glm::vec2 mousePos = ControlCenter::getInstance().getCursorPos(window);
            value = minVal + (mousePos.x - mouseStartDrag - getPosition().x)/(width-0.2)*(maxVal - minVal);
            value = std::clamp(value, minVal, maxVal);
            update();
            is_changed = true;
        }
        return is_changed;
    }
    void Slider::glDraw() const {
        ShapeContainer::draw(__rectangle);
        Container2D::glDraw();
        //Text::draw();
    }
    bool Slider::contains(const glm::vec2& position) const {
        return __rectangle.contains(position-getPosition());
    }
    void Slider::update(){
        __scrollButton.setPosition({getPosition().x + (width-0.2)*(value - minVal)/(maxVal - minVal), getPosition().y + height*0.05f});
        Text::setPosition({getPosition().x + width/2 - Text::getSize().x/2, getPosition().y - height});
        Text::setText(std::to_string(value));
    }

}
