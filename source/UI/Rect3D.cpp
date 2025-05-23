#include "Rect3D.h"
#include "General.h"
#include "Line.h"
#include "Ray.h"
#include "SFML/System/Vector3.hpp"
#include "Segment.h"
Rect3f::Rect3f() {
    pA = pB = pC = {0, 0, 0};
}
Rect3f::Rect3f(const Vector3f& a, const Vector3f& b, const Vector3f& c) {
    pA = a; pB = b; pC = c;
}
Rect3f::~Rect3f() {

}
bool Rect3f::isIntersect(const Ray3f& ray) const {
    Vector3f normal = det(pB-pA, pC-pB);
    if (ray*normal == 0) return false;
    float normalSide = ray*normal/abs(normal);
    float times = abs(distance(ray.getOrigin())/normalSide);
    Vector3f pos = ray.getOrigin() + times*(Vector3f)ray;
    return contains(pos);
}
bool Rect3f::contains(const Vector3f& position) const {
    Segment AB(pA, pB);
    Segment BC(pB, pC);
    Segment CD(pC, operator[](3));
    Segment DA(operator[](3), pA);
    if (abs(AB.distance(position)+CD.distance(position) - BC.length())<0.01 && abs(BC.distance(position) + DA.distance(position) - AB.length())<0.01)
        return true;
    return false;
}
float Rect3f::distance(const Vector3f& position) const {
    Vector3f delta = position-pA;
    Vector3f normal = det(pB-pA, pC-pB);
    return delta*normal/abs(normal);
}
Vector3f Rect3f::operator[](const uint& index) const {
    switch (index) {
    case 0: return pA;
    case 1: return pB;
    case 2: return pC;
    default: return pC+pA-pB;
    }
}
void Rect3f::move(const Vector3f& offset) {
    pA += offset;
    pB += offset;
    pC += offset;
}
Vector3f Rect3f::getCenter() const {
    return (pA+pC)/2.f;
}
Vector3f Rect3f::getNormal() const {
    return det(pB-pA, pC-pB);
}
Vector3f Rect3f::getIntersect(const Ray3f& ray) const {
    Vector3f normal = det(pB-pA, pC-pB);
    float normalSide = ray*normal/abs(normal);
    float times = abs(distance(ray.getOrigin())/normalSide);
    Vector3f pos = ray.getOrigin() + times*(Vector3f)ray;
    return pos;
}