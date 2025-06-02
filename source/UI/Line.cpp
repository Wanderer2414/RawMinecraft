#include "Line.h"
#include "General.h"
#include "glm/geometric.hpp"
namespace MyBase3D {

    Line::Line(const glm::vec3& origin,  const glm::vec3& delta):_origin(origin), _delta(delta) {
    }
    Line::~Line() {
    
    }
    
    bool Line::contains(const glm::vec3& position) const {
        if (position == _origin) return true;
        glm::vec3 delta = position-_origin;
        delta = delta/glm::length(delta)*glm::length(_delta);
        return delta == _delta || -delta == _delta;
    }
    float Line::distance(const glm::vec3& position) const {
        return glm::length(det(position-_origin, _delta))/glm::length(_delta);
    } 
}