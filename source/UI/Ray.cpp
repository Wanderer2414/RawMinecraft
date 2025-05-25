#include "Ray.h"
#include "General.h"

Ray3f::Ray3f(const glm::vec3& origin, const glm::vec3& des) {
    pOrigin = origin;
    pDelta = des - origin;
}

Ray3f::~Ray3f() {

}
glm::vec3 Ray3f::getOrigin() const {
    return pOrigin;
}

Ray3f::operator glm::vec3() const {
    return pDelta;
}