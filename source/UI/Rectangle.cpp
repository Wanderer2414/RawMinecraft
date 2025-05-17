#include "Rectangle.h"
#include "BaseShape.h"
#include "Global.h"

Rectangle::Rectangle() {
    m_size = {100, 50};
    update();
};
bool Rectangle::contains(const sf::Vector2f& point) const {
    return getGlobalBounds().contains(point);
}

size_t Rectangle::getPointCount() const {
    return 4;
}
sf::Vector2f Rectangle::getPoint(size_t index) const {
    switch (index) {
        case 0: return {0, 0}; break;
        case 1: return {m_size.x, 0}; break;
        case 2: return m_size; break;
        default: return {0, m_size.y}; break;
    }
}

void Rectangle::setSize(const float& x, const float& y) {
    m_size = {x, y};
    update();
}
void Rectangle::update() {
    Shape::update();
}
Vector2f Rectangle::getCenter() const {
    return Shape::getPosition()+m_size/2.f;
}

Vector2f Rectangle::getSize() const {
    return m_size;
}
