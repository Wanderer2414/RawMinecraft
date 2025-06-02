#include "Plane.h"
#include "General.h"
namespace MyBase3D {

    Plane::Plane(const glm::vec3& origin, const glm::vec3& normal):__origin(origin), __normal(normal) {
    }
    Plane::~Plane() {
    
    }
    Plane::operator glm::vec3() const {
        return __normal;
    }
    float Plane::distance(const glm::vec3& position) const {
        glm::vec3 delta = position-__origin;
        return delta*__normal/glm::length(__normal);
    }
    glm::vec3 Plane::isIntersect(const Ray3f& ray) const {
        float normalSide = ray*__normal/glm::length(__normal);
        float times = distance(ray.getOrigin())/normalSide;
        return ray.getOrigin() + times*(glm::vec3)ray;
    }
    glm::vec3 Plane::getOrigin() const {
        return __origin;
    }
}