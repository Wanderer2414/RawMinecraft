#include "Rectangle.h"
namespace MyBase {

    Rectangle::Rectangle() {
        __size = {100, 50};
        update();
    };
    Rectangle::~Rectangle() {
        
    }
    bool Rectangle::contains(const sf::Vector2f& point) const {
        return getGlobalBounds().contains(point);
    }
    
    std::size_t Rectangle::getPointCount() const {
        return 4;
    }
    sf::Vector2f Rectangle::getPoint(std::size_t index) const {
        switch (index) {
            case 0: return {0, 0}; break;
            case 1: return {__size.x, 0}; break;
            case 2: return __size; break;
            default: return {0, __size.y}; break;
        }
    }
    
    void Rectangle::setSize(const float& x, const float& y) {
        __size = {x, y};
        update();
    }
    void Rectangle::setSize(const sf::Vector2f& size) {
        setSize(size.x, size.y);
    }
    void Rectangle::update() {
        Shape::update();
    }
    sf::Vector2f Rectangle::getCenter() const {
        return Shape::getPosition()+__size/2.f;
    }
    
    sf::Vector2f Rectangle::getSize() const {
        return __size;
    }
    
}