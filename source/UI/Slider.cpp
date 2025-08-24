#include "Slider.h"
#include "ControlCenter.h"
#include "Shape.h"
#include "Global.h"
#include "Rectangle.h"
#include "ShapeManager.h"
#include <iostream>

namespace MyBase  {

    Slider::Slider(): width(1.0f), maxVal(100), minVal(0), value(50), isScroll(false) {
        ShapeManager::getInstance().createShape(__rectangle, {width, height});
        ShapeContainer::setFillColor({200, 200, 200, 255});

        __scrollButton.setNormalColor(WHITE);
        __scrollButton.setHoverColor({100,100,100,255});
        __scrollButton.setClickColor({50,50,50,255});

        // nút trượt chiếm 10% chiều ngang
        __scrollButton.setSize({width * 0.1f, height * 0.9f});
        insert(&__scrollButton);

        Text::setScale({0.05f, 0.05f});
        Text::setTextColor(BLACK);

        update();
    }

    Slider::~Slider(){
        ShapeManager::getInstance().removeShape(__rectangle, {width, height});
    }

    void Slider::setPosition(const glm::vec2& position){
        ShapeContainer::setPosition(position);
        update();
    }

    void Slider::setWidth(const float& w){
        if(this->width != w){
            ShapeManager::getInstance().removeShape(__rectangle, {width, height});
            this->width = w;
            ShapeManager::getInstance().createShape(__rectangle, {width, height});
            __scrollButton.setSize({width * 0.1f, height * 0.9f}); // cập nhật size nút trượt
            update();
        }
    }

    void Slider::setMaxValue(const int & v){
        if(v > minVal){
            maxVal = v;
            update();
        }
    }

    void Slider::setMinValue(const int & v){
        if(v < maxVal){
            minVal = v;
            update();
        }
    }

    void Slider::setText(const std::string& text){
        Text::setText(text);
        Text::setPosition({
            getPosition().x + width/2 - Text::getSize().x/2,
            getPosition().y - height
        });
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

        // xử lý cuộn chuột
        float xScroll = ControlCenter::getInstance().getScroll().x / 2.f;
        if(xScroll){
            xScroll *= (maxVal - minVal) / 10.f;
            value += (int)xScroll;
            value = std::clamp(value, minVal, maxVal);
            update();
            is_changed = true;
        }

        // xử lý drag
        if(__scrollButton.isPressed()){
            isScroll = true;
            mouseStartDrag = ControlCenter::getInstance().getCursorPos(window).x - __scrollButton.getPosition().x;
        }
        if(__scrollButton.isReleased()) isScroll = false;

        if(isScroll){
            glm::vec2 mousePos = ControlCenter::getInstance().getCursorPos(window);
            float usableWidth = width - __scrollButton.getSize().x;
            float ratio = (mousePos.x - mouseStartDrag - getPosition().x) / usableWidth;
            value = minVal + ratio * (maxVal - minVal);
            value = std::clamp(value, minVal, maxVal);
            update();
            is_changed = true;
        }

        return is_changed;
    }

    void Slider::glDraw() const {
        ShapeContainer::draw(__rectangle);
        Container2D::glDraw();
        Text::draw();
    }

    bool Slider::contains(const glm::vec2& position) const {
        // dùng top-left nên không cần chỉnh sửa
        return __rectangle.contains(position - getPosition());
    }

    void Slider::update(){
        float usableWidth = width - __scrollButton.getSize().x;

        // canh NÚT theo top-left, đặt Y ở giữa slider
        __scrollButton.setPosition({
            getPosition().x + usableWidth * (float)(value - minVal) / (float)(maxVal - minVal),
            getPosition().y + (height - __scrollButton.getSize().y) / 2.0f
        });

        Text::setPosition({
            __scrollButton.getPosition().x + width/2 - Text::getSize().x/2,
            __scrollButton.getPosition().y - height
        });

        Text::setText(std::to_string(value));

        std::cout<<"Button pos: "<<__scrollButton.getPosition().x<<", "<<__scrollButton.getPosition().y<<"\n";
        std::cout<<"Slider val: "<<value<<"\n";
    }

}
