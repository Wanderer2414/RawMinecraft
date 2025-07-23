#include "Textbox.h"
#include "ControlCenter.h"
#include "Shape.h"
#include "ShapeManager.h"

namespace MyBase {
    Textbox::Textbox(): __size(0,0), __roundness(0), __isCursorShow(false) {
        ShapeManager::getInstance().createShape(__bound, __size, __roundness);
        ShapeManager::getInstance().createShape(__textCursor, glm::vec2(0.002, __size.y*0.7));
        __textCursorContainer.setPosition(getPosition()+glm::vec2(0.01, 0));
        Text::setPosition(getPosition()+glm::vec2(0.01, 0));
        setMarginColor(BLACK);
        __textCursorContainer.setFillColor(BLACK);
        __cursorClock.setDuration(500);
    }
    Textbox::~Textbox() {
        ShapeManager::getInstance().removeShape(__bound, __size, __roundness);
        ShapeManager::getInstance().removeShape(__textCursor, glm::vec2(0.002, __size.y*0.7));
    }

    glm::vec2 Textbox::getPosition() const {
        return ShapeContainer::getPosition();
    };
    glm::vec2 Textbox::getSize() const {
        return __size;
    };
    void Textbox::setText(const std::string& text) {
        Text::setText(text);
        update();
    }
    void Textbox::setSize(const glm::vec2& size, const float& roundness) {
        if (size!=__size || __roundness != roundness) {
            ShapeManager::getInstance().removeShape(__bound, __size, __roundness);
            ShapeManager::getInstance().removeShape(__textCursor, glm::vec2(0.002, __size.y*0.7));
            __size = size; __roundness = roundness;
            ShapeManager::getInstance().createShape(__bound, __size, __roundness);
            ShapeManager::getInstance().createShape(__textCursor, glm::vec2(0.002, __size.y*0.7));
            __textCursorContainer.setPosition(getPosition()+glm::vec2(0.01, __size.y*0.15));
            Text::setPosition(getPosition()+glm::vec2(0.01, (__size.y-Text::getSize().y)/2));
        }
    };
    void Textbox::update() {
        if (Text::getSize().x < __size.x) {
            Text::setPosition(getPosition()+glm::vec2(0.01, (__size.y-Text::getSize().y)/2));
            __textCursorContainer.setPosition(getPosition()+glm::vec2(Text::getSize().x + 0.01, __size.y*0.15));
        } 
        else {
            Text::setPosition(getPosition()+glm::vec2(__size.x - Text::getSize().x - 0.01, (__size.y-Text::getSize().y)/2));
            __textCursorContainer.setPosition(getPosition()+glm::vec2(getSize().x - 0.01, __size.y*0.15));
        }
    }
    void Textbox::setScale(const glm::vec2& scale) {
        Text::setScale(scale);
        Text::setPosition(getPosition()+glm::vec2(0.01, (__size.y-Text::getSize().y)/2));
    }
    void Textbox::setPosition(const glm::vec2& position) {
        __textCursorContainer.move(position - getPosition());
        Text::move(position - getPosition());
        ShapeContainer::setPosition(position);
    };
    bool Textbox::__focus(GLFWwindow*) {
        setMarginWidth(2);
        return true;
    }
    bool Textbox::__onFocus(GLFWwindow*) {
        if (__cursorClock.get()) {
            __cursorClock.restart();
            __isCursorShow = !__isCursorShow;
            return true;
        }
        if (ControlCenter::getInstance().IsKeyPressed() && ControlCenter::getInstance().GetCharInput()) {
            if (ControlCenter::getInstance().GetCharInput()=='\b') Text::setText(Text::getText().substr(0, Text::getText().size()-1));
            else Text::setText(Text::getText()+ControlCenter::getInstance().GetCharInput());
            __isCursorShow = true;
            update();
            return true;
        }
        return false;
    }
    bool Textbox::__lostFocus(GLFWwindow*) {
        setMarginWidth(0);
        __isCursorShow = false;
        return true;
    }
    bool Textbox::contains(const glm::vec2& position) const {
        return __bound.contains(position-getPosition());
    };
    void Textbox::glDraw() const {
        ShapeContainer::draw(__bound);
        Text::draw();
        if (__isCursorShow) __textCursorContainer.draw(__textCursor);
    }
}