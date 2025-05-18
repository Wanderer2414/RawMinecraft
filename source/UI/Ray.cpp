#include "Ray.h"
#include "General.h"

Ray3f::Ray3f(const Vector3f& origin, const Vector3f& des) {
    pOrigin = origin;
    pDelta = des - origin;
    pDelta /= abs(pDelta);
}

Ray3f::~Ray3f() {

}
Vector3f Ray3f::getOrigin() const {
    return pOrigin;
}

Ray3f::operator Vector3f() const {
    return pDelta;
}