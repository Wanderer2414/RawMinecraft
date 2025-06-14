#ifndef PLANE_H
#define PLANE_H
#include "Ray.h"
namespace MyBase3D {
    class Plane {
        public:
            Plane(const glm::vec3& origin, const glm::vec3& normal);
            ~Plane();
        
            operator        glm::vec3() const;
            float           distance(const glm::vec3& position) const;
            glm::vec3       isIntersect(const Ray3f& ray) const;
            glm::vec3       getOrigin() const;  
        private:
            glm::vec3    __origin, __normal;
        };
}
#endif