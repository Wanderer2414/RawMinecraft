#include "Circle.h"
#include "General.h"

namespace MyBase {

    Circle::Circle() {
        __radius = 20;
        __pointCount = 30;
    }
    Circle::~Circle() {
    
    }
    bool Circle::contains(const sf::Vector2f& point) const {
        return abs(getPosition() - point)<=__radius;
    }
    
    std::size_t Circle::getPointCount() const {
        return __pointCount;
    }
    
    void Circle::setPointCount(const size_t& pointCount) {
        __pointCount = pointCount;
        update();
    }
    void Circle::setRadius(const float& radius) {
        __radius = radius;
        update();
    }
    void Circle::update() {
        Shape::update();
    }
    sf::Vector2f Circle::getPoint(size_t index) const {
        float angle = 2.0f*M_PI*index/__pointCount;
        return __radius*sf::Vector2f(cos(angle), sin(angle));
    }
    sf::Vector2f Circle::getCenter() const {
        return getPosition();
    }
    sf::Vector2f Circle::getSize() const {
        return {__radius, __radius};
    }
}