#include "Segment.h"
#include "General.h"

Segment::Segment(const Vector3f& source, const Vector3f& destination): Line(source, destination-source) {

}
Segment::~Segment() {

}

bool Segment::contains(const Vector3f& position) const {
    if (position == sOrigin) return true;
    Vector3f delta = position-sOrigin;
    float length_delta = abs(delta), length_sDelta = abs(sDelta);
    delta = delta/length_delta*length_sDelta;
    return delta == sDelta && length_delta <= length_sDelta;
}
float Segment::length() const {
    return abs(sDelta);
}