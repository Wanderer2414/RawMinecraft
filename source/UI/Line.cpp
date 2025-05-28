#include "Line.h"
#include "General.h"
#include "glm/geometric.hpp"
namespace MyBase3D {

    Line::Line(const glm::vec3& origin,  const glm::vec3& delta) {
        sOrigin = origin;
        sDelta = delta;
    }
    Line::~Line() {
    
    }
    
    bool Line::contains(const glm::vec3& position) const {
        if (position == sOrigin) return true;
        glm::vec3 delta = position-sOrigin;
        delta = delta/glm::length(delta)*glm::length(sDelta);
        return delta == sDelta || -delta == sDelta;
    }
    float Line::distance(const glm::vec3& position) const {
        return glm::length(det(position-sOrigin, sDelta))/glm::length(sDelta);
    } 
}