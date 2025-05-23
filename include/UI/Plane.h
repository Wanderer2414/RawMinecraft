#ifndef PLANE_H
#define PLANE_H
#include "Ray.h"
class Plane {
public:
    Plane(const Vector3f& origin, const Vector3f& normal);
    ~Plane();

    operator Vector3f() const;
    float       distance(const Vector3f& position) const;
    Vector3f    isIntersect(const Ray3f& ray) const;
    Vector3f    getOrigin() const;  
private:
    Vector3f    pOrigin, pNormal;
};
#endif