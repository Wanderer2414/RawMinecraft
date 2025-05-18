#include "Line.h"
#include "General.h"

Line::Line(const Vector3f& origin,  const Vector3f& delta) {
    sOrigin = origin;
    sDelta = delta;
}
Line::~Line() {

}

bool Line::contains(const Vector3f& position) const {
    if (position == sOrigin) return true;
    Vector3f delta = position-sOrigin;
    delta = delta/abs(delta)*abs(sDelta);
    return delta == sDelta || -delta == sDelta;
}
float Line::distance(const Vector3f& position) const {
    return abs(det(sDelta, position-sOrigin))/abs(sDelta);
}