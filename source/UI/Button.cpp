#include "Button.h"
#include "RoundedRectangle.h"
#include "Shape.h"
#include "ShapeManager.h"

namespace MyBase {

    template <typename T>
    Button<T>::Button() {}

    template <typename T>
    Button<T>::~Button() {}

    template <typename T>
    bool Button<T>::contains(const glm::vec2& position) const {
        return getShape().contains(position - getPosition());
    }
    template <typename T>
    void Button<T>::setNormalColor(const Color& color) {
        __normalColor = color;
        setFillColor(color);
    }
    template <typename T>
    void Button<T>::setHoverColor(const Color& color) {
        __hoverColor = color;
    }
    template <typename T>
    void Button<T>::setClickColor(const Color& color) {
        __clickColor = color;
    }
    template <typename T>
    void Button<T>::setPosition(const glm::vec2& position) {
        Text::move(position-getPosition());
        ShapeContainer::setPosition(position);
    }
    template <typename T>
    void Button<T>::setText(const std::string& text) {
        Text::setText(text);
        update();
    }
    template <typename T>
    void Button<T>::setScale(const glm::vec2& scale) {
        Text::setScale(scale);
        update();
    }
    template <typename T>
    void Button<T>::move(const glm::vec2& offset) {
        ShapeContainer::move(offset);
        Text::move(offset);
    }
    template <typename T>
    void Button<T>::setFont(const Font& font) {
        Text::setFont(font);
        update();
    }
    template <typename T>
    void Button<T>::update() {
        Text::setPosition(getPosition()+getShape().getCenter()-Text::getSize()/2.f);
    }

    template<typename T>
    glm::vec2 Button<T>::getSize() const {
        return getShape().getSize();
    }
    template<typename T>
    glm::vec2 Button<T>::getPosition() const {
        return ShapeContainer::getPosition();
    }
    template <typename T>
    bool Button<T>::__lostHover() {
        setFillColor(__normalColor);
        return true;
    }

    template <typename T>
    bool Button<T>::__hover() {
        setFillColor(__hoverColor);
        return true;
    }

    template <typename T>
    bool Button<T>::__mouseDown(GLFWwindow*) {
        setFillColor(__clickColor);
        return true;
    }

    template <typename T>
    bool Button<T>::__mouseRelease(GLFWwindow*) {
        setFillColor(__hoverColor);
        return true;
    }

    template <typename T>
    void Button<T>::glDraw() const {
        ShapeContainer::draw(getShape());;
        Text::draw();
    }

    RectangleButton::RectangleButton(): __size(0,0) {
        ShapeManager::getInstance().createShape(getShape(), __size);
    }
    RectangleButton::~RectangleButton()  {
        ShapeManager::getInstance().removeShape(getShape(), __size);
    };

    void RectangleButton::setSize(const glm::vec2& size) {
        if (size!=__size) {
            ShapeManager::getInstance().removeShape(getShape(), __size);
            __size = size;
            ShapeManager::getInstance().createShape(getShape(), size);
            update();
        }
    }

    Rectangle& RectangleButton::getShape() {
        return __shape;
    };
    const Rectangle& RectangleButton::getShape() const {
        return __shape;
    };

    RoundedRectangleButton::RoundedRectangleButton(): __size(0,0), __radius(0) {
        ShapeManager::getInstance().createShape(__shape, __size, __radius);
    }
    RoundedRectangleButton::~RoundedRectangleButton() {
        ShapeManager::getInstance().removeShape(getShape(), __size, __radius);
    }
    void RoundedRectangleButton::setSize(const glm::vec2& size, const float& radius) {
        if (__size!=size || __radius != radius) {
            ShapeManager::getInstance().removeShape(getShape(), __size, __radius);
            __size = size;
            __radius = radius;
            ShapeManager::getInstance().createShape(__shape, __size, __radius);
            update();
        }
    }
    RoundedRectangle& RoundedRectangleButton::getShape() {
        return __shape;
    }
    const RoundedRectangle& RoundedRectangleButton::getShape() const {
        return __shape;
    }

    EllipseButton::EllipseButton():__size(0,0) {
        ShapeManager::getInstance().createShape(__shape, __size);
    }
    EllipseButton::~EllipseButton() {
        ShapeManager::getInstance().removeShape(getShape(), __size);
    }
    void EllipseButton::setSize(const glm::vec2& size) {
        if (size!=__size) {
            ShapeManager::getInstance().removeShape(getShape(), __size);
            __size = size;
            ShapeManager::getInstance().createShape(__shape, __size);
            update();
        }
    }
    Ellipse& EllipseButton::getShape() {
        return __shape;
    }
    const Ellipse& EllipseButton::getShape() const {
        return __shape;
    }
}