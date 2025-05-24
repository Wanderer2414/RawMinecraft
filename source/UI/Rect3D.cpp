#include "Rect3D.h"
#include "General.h"
#include "Ray.h"
#include "Segment.h"
#include "glm/geometric.hpp"
Rect3f::Rect3f() {
    pA = pB = pC = {0, 0, 0};
}
Rect3f::Rect3f(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    pA = a; pB = b; pC = c;
}
Rect3f::~Rect3f() {

}
bool Rect3f::isIntersect(const Ray3f& ray) const {
    glm::vec3 normal = det(pB-pA, pC-pB);
    if (ray*normal == 0) return false;
    float normalSide = ray*normal/glm::length(normal);
    float times = abs(distance(ray.getOrigin())/normalSide);
    glm::vec3 pos = ray.getOrigin() + times*(glm::vec3)ray;
    return contains(pos);
}
bool Rect3f::contains(const glm::vec3& position) const {
    Segment AB(pA, pB);
    Segment BC(pB, pC);
    Segment CD(pC, operator[](3));
    Segment DA(operator[](3), pA);
    if (abs(AB.distance(position)+CD.distance(position) - BC.length())<0.01 && abs(BC.distance(position) + DA.distance(position) - AB.length())<0.01)
        return true;
    return false;
}
float Rect3f::distance(const glm::vec3& position) const {
    glm::vec3 delta = position-pA;
    glm::vec3 normal = det(pB-pA, pC-pB);
    return delta*normal/glm::length(normal);
}
glm::vec3 Rect3f::operator[](const uint& index) const {
    switch (index) {
    case 0: return pA;
    case 1: return pB;
    case 2: return pC;
    default: return pC+pA-pB;
    }
}
void Rect3f::move(const glm::vec3& offset) {
    pA += offset;
    pB += offset;
    pC += offset;
}
glm::vec3 Rect3f::getCenter() const {
    return (pA+pC)/2.f;
}
glm::vec3 Rect3f::getNormal() const {
    return det(pB-pA, pC-pB);
}
glm::vec3 Rect3f::getIntersect(const Ray3f& ray) const {
    glm::vec3 normal = det(pB-pA, pC-pB);
    float normalSide = ray*normal/glm::length(normal);
    float times = abs(distance(ray.getOrigin())/normalSide);
    glm::vec3 pos = ray.getOrigin() + times*(glm::vec3)ray;
    return pos;
}