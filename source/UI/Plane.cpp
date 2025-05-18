#include "Plane.h"
#include "General.h"
Plane::Plane(const Vector3f& origin, const Vector3f& normal) {
    pOrigin = origin;
    pNormal = normal;
}
Plane::~Plane() {

}
Plane::operator Vector3f() const {
    return pNormal;
}
float Plane::distance(const Vector3f& position) const {
    Vector3f delta = position-pOrigin;
    return delta*pNormal/abs(pNormal);
}
Vector3f Plane::isIntersect(const Ray3f& ray) const {
    float normalSide = ray*pNormal/abs(pNormal);
    float times = distance(ray.getOrigin())/normalSide;
    return ray.getOrigin() + times*(Vector3f)ray;
}
Vector3f Plane::getOrigin() const {
    return pOrigin;
}