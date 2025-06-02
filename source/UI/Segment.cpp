#include "Segment.h"
#include "General.h"
namespace MyBase3D {
    Segment::Segment(const glm::vec3& source, const glm::vec3& destination): Line(source, destination-source) {

    }
    Segment::~Segment() {
    
    }
    
    bool Segment::contains(const glm::vec3& position) const {
        if (position == _origin) return true;
        glm::vec3 delta = position-_origin;
        float length_delta = glm::length(delta), length_sDelta = glm::length(_delta);
        delta = delta/length_delta*length_sDelta;
        return delta == _delta && length_delta <= length_sDelta;
    }
    float Segment::length() const {
        return glm::length(_delta);
    }
}