#ifndef RAY_H
#define RAY_H
#include "Global.h"
class Ray3f {
public:
    Ray3f(const Vector3f& origin, const Vector3f& des);
    ~Ray3f();

    operator    Vector3f() const;
    Vector3f    getOrigin() const;
private:
    Vector3f        pOrigin, pDelta;
};
#endif