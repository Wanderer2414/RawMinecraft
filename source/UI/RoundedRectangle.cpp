#include "RoundedRectangle.h"
#include "Global.h"
namespace MyBase {

    RoundedRectangle::RoundedRectangle(): __size(0, 0), __radius(0), __pointEachCount(8) {};
    RoundedRectangle::RoundedRectangle(const glm::vec2& size, const float& radius):
        __pointEachCount(10), __size(size), __radius(radius) {
        update();
    }
    bool RoundedRectangle::contains(const glm::vec2& position) const {
        return (position.x>=0 && position.x<=__size.x)
            && (position.y>=0 && position.y<=__size.y);
    }
    
    size_t RoundedRectangle::getPointCount() const {
        return __pointEachCount*4;
    }
    glm::vec2 RoundedRectangle::getPoint(const size_t& index) const {
        if (!index) return {0, __radius};
        else if (index < __pointEachCount - 1) {
            float angle = M_PI/2*index/__pointEachCount;
            return glm::vec2(__radius - __radius*cos(angle), __radius - __radius*sin(angle));
        }
        else if (index < __pointEachCount) return {__radius, 0};
        else if (index < __pointEachCount+1) return {__size.x - __radius, 0};
        else if (index < 2*__pointEachCount-1) {
            float angle = M_PI/2*index/__pointEachCount;
            return glm::vec2(__size.x - __radius - __radius*cos(angle), __radius - __radius*sin(angle));
        }
        else if (index < 2*__pointEachCount) return {__size.x,__radius};
        else if (index < 2*__pointEachCount+1) return {__size.x, __size.y-__radius};
        else if (index < 3*__pointEachCount-1) {
            float angle = M_PI/2*index/__pointEachCount;
            return glm::vec2(__size.x - __radius - __radius*cos(angle), __size.y - __radius - __radius*sin(angle));
        }
        else if (index < 3*__pointEachCount) return {__size.x - __radius, __size.y};
        else if (index < 3*__pointEachCount+1) return {__radius, __size.y};
        else if (index < 4*__pointEachCount-1) {
            float angle = M_PI/2*index/__pointEachCount;
            return glm::vec2(__radius - __radius*cos(angle), __size.y - __radius - __radius*sin(angle));
        }
        else return {0, __size.y - __radius};
    }
    
    glm::vec2 RoundedRectangle::getSize() const {
        return __size;
    }
    glm::vec2 RoundedRectangle::getCenter() const {
        return __size/2.f;
    }
}