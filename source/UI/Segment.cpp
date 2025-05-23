#include "Segment.h"
#include "General.h"

Segment::Segment(const glm::vec3& source, const glm::vec3& destination): Line(source, destination-source) {

}
Segment::~Segment() {

}

bool Segment::contains(const glm::vec3& position) const {
    if (position == sOrigin) return true;
    glm::vec3 delta = position-sOrigin;
    float length_delta = abs(delta), length_sDelta = abs(sDelta);
    delta = delta/length_delta*length_sDelta;
    return delta == sDelta && length_delta <= length_sDelta;
}
float Segment::length() const {
    return abs(sDelta);
}