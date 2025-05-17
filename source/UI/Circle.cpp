#include "Circle.h"
#include "General.h"
#include "Global.h"
#include "SFML/System/Vector2.hpp"

Circle::Circle() {
    m_radius = 20;
    m_point_count = 30;
}
Circle::~Circle() {

}
bool Circle::contains(const Vector2f& point) const {
    return abs(getPosition() - point)<=m_radius;
}

size_t Circle::getPointCount() const {
    return m_point_count;
}

void Circle::setPointCount(const size_t& pointCount) {
    m_point_count = pointCount;
    update();
}
void Circle::setRadius(const float& radius) {
    m_radius = radius;
    update();
}

Vector2f Circle::getPoint(size_t index) const {
    float angle = 2.0f*M_PI*index/m_point_count;
    return m_radius*Vector2f(cos(angle), sin(angle));
}
Vector2f Circle::getCenter() const {
    return getPosition();
}
Vector2f Circle::getSize() const {
    return {m_radius, m_radius};
}