#include "Plane.h"
#include "General.h"
Plane::Plane(const glm::vec3& origin, const glm::vec3& normal) {
    pOrigin = origin;
    pNormal = normal;
}
Plane::~Plane() {

}
Plane::operator glm::vec3() const {
    return pNormal;
}
float Plane::distance(const glm::vec3& position) const {
    glm::vec3 delta = position-pOrigin;
    return delta*pNormal/glm::length(pNormal);
}
glm::vec3 Plane::isIntersect(const Ray3f& ray) const {
    float normalSide = ray*pNormal/glm::length(pNormal);
    float times = distance(ray.getOrigin())/normalSide;
    return ray.getOrigin() + times*(glm::vec3)ray;
}
glm::vec3 Plane::getOrigin() const {
    return pOrigin;
}