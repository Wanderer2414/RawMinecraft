#include "Line.h"
#include "General.h"

Line::Line(const glm::vec3& origin,  const glm::vec3& delta) {
    sOrigin = origin;
    sDelta = delta;
}
Line::~Line() {

}

bool Line::contains(const glm::vec3& position) const {
    if (position == sOrigin) return true;
    glm::vec3 delta = position-sOrigin;
    delta = delta/abs(delta)*abs(sDelta);
    return delta == sDelta || -delta == sDelta;
}
float Line::distance(const glm::vec3& position) const {
    return abs(det(position-sOrigin, sDelta))/abs(sDelta);
} 