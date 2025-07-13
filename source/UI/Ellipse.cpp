#include "Ellipse.h"
namespace MyBase {
    Ellipse::Ellipse():__size(0,0) {
        update();
    }
    Ellipse::Ellipse(const glm::vec2& size):__size(size) {
        update();
    }
    Ellipse::~Ellipse() {}

    bool Ellipse::contains(const glm::vec2& position) const {
        glm::vec2 delta = 1.f-2.f*position/__size;
        return glm::length(delta)<=1;
    }
    size_t Ellipse::getPointCount() const {
        return pointCount;
    }
    glm::vec2 Ellipse::getPoint(const size_t& index) const {
        float angle = 2*M_PI/pointCount*(index+1);
        return __size/2.f + glm::vec2(__size.x/2*std::cos(angle), __size.y/2*std::sin(angle));
    }
    glm::vec2 Ellipse::getCenter() const {
        return __size/2.f;
    }
    glm::vec2 Ellipse::getSize() const {
        return __size;
    }
}