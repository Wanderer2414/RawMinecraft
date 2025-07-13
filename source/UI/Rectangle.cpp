#include "Rectangle.h"
namespace MyBase {
    Rectangle::Rectangle(): __size(0,0) {}
    Rectangle::Rectangle(const glm::vec2& size): __size(size) {
        update();
    }
    Rectangle::~Rectangle() {}

    bool Rectangle::contains(const glm::vec2& position) const {
        return (position.x>=0 && position.x<=__size.x)
            && (position.y>=0 && position.y<=__size.y);
    }
    size_t Rectangle::getPointCount() const {
        return 4;
    }
    glm::vec2 Rectangle::getSize() const {
        return __size;
    }
    glm::vec2 Rectangle::getCenter() const {
        return __size/2.f;
    }
    glm::vec2 Rectangle::getPoint(const size_t& index) const {
        switch (index) {
            case 0: return glm::vec2(__size.x, 0);
            case 1: return glm::vec2(__size.x, __size.y);
            case 2: return glm::vec2(0, __size.y);
            default: return glm::vec2(0, 0);
        }
    }
}