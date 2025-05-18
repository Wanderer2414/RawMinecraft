#ifndef RECT3D_H
#define RECT3D_H
#include "Global.h"
#include "Plane.h"
#include "Ray.h"

class Rect3f {
public:
    Rect3f();
    Rect3f(const Vector3f& a, const Vector3f& b, const Vector3f& c);
    ~Rect3f();

    bool        isIntersect(const Ray3f& ray)       const,
                contains(const Vector3f& position)  const;
    float       distance(const Vector3f& position)  const;
    void        move(const Vector3f& offset);
    Vector3f    getCenter()                     const,
                getNormal()                     const,
                operator[](const uint& index)   const;
private:
    Vector3f    pA, pB, pC;
};
#endif