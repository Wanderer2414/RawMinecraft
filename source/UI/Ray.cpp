#include "Ray.h"
#include "General.h"
#include <cmath>
namespace MyBase3D {

Ray3f::Ray3f(const glm::vec3& origin, const glm::vec3& des):__origin(origin), __delta(des-origin) {
}

Ray3f::~Ray3f() {

}
glm::vec3 Ray3f::distance(const char& plane, const float& value) const {
    if (!plane) {
        float dis = value-__origin.x;
        if (!dis) return {0, 0, 0};
        else {
            float times = dis/__delta.x;
            if (times>0) return __delta*times;
            else return glm::vec3(INFINITY);
        }
    }
    else if (plane==1) {
        float dis = value-__origin.y;
        if (!dis) return {0, 0, 0};
        else {
            float times = dis/__delta.y;
            if (times>0) return __delta*times;
            else return glm::vec3(INFINITY);
        }
    }
    else if (plane==2) {
        float dis = value-__origin.z;
        if (!dis) return {0, 0, 0};
        else {
            float times = dis/__delta.z;
            if (times>0) return __delta*times;
            else return glm::vec3(INFINITY);
        }
    }
    return {0, 0, 0};
}
glm::vec3 Ray3f::getOrigin() const {
    return __origin;
}

Ray3f::operator glm::vec3() const {
    return __delta;
}
}