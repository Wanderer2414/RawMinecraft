#include "Ray.h"
#include "General.h"
#include <cmath>

Ray3f::Ray3f(const glm::vec3& origin, const glm::vec3& des) {
    pOrigin = origin;
    pDelta = des - origin;
}

Ray3f::~Ray3f() {

}
glm::vec3 Ray3f::distance(const char& plane, const float& value) const {
    if (!plane) {
        float dis = value-pOrigin.x;
        if (!dis) return {0, 0, 0};
        else {
            float times = dis/pDelta.x;
            if (times>0) return pDelta*times;
            else return glm::vec3(INFINITY);
        }
    }
    else if (plane==1) {
        float dis = value-pOrigin.y;
        if (!dis) return {0, 0, 0};
        else {
            float times = dis/pDelta.y;
            if (times>0) return pDelta*times;
            else return glm::vec3(INFINITY);
        }
    }
    else if (plane==2) {
        float dis = value-pOrigin.z;
        if (!dis) return {0, 0, 0};
        else {
            float times = dis/pDelta.z;
            if (times>0) return pDelta*times;
            else return glm::vec3(INFINITY);
        }
    }
    return {0, 0, 0};
}
glm::vec3 Ray3f::getOrigin() const {
    return pOrigin;
}

Ray3f::operator glm::vec3() const {
    return pDelta;
}