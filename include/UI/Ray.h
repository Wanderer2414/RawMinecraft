#ifndef RAY_H
#define RAY_H
#include "Global.h"
namespace MyBase3D {
    class Ray3f {
        public:
            Ray3f(const glm::vec3& origin, const glm::vec3& des);
            ~Ray3f();

            operator    glm::vec3() const;
            glm::vec3   distance(const char& plane, const float& value) const;
            glm::vec3    getOrigin() const;
        private:
            glm::vec3        __origin, __delta;
        };
};
#endif