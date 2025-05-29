#ifndef RECT3D_H
#define RECT3D_H
#include "Global.h"
#include "Plane.h"
#include "Ray.h"
namespace MyBase3D {
class Rect3f {
    public:
        Rect3f();
        Rect3f(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
        ~Rect3f();
    
        bool        isIntersect(const Ray3f& ray)       const,
                    contains(const glm::vec3& position)  const;

        float       distance(const glm::vec3& position)  const;

        void        move(const glm::vec3& offset);
        glm::vec3   getCenter()                     const,
                    getNormal()                     const,
                    getIntersect(const Ray3f& sight) const,
                    operator[](const uint& index)   const;
    private:
        glm::vec3    __A, __B, __C;
    };
}
#endif