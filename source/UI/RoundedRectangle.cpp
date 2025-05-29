#include "RoundedRectangle.h"
namespace MyBase {

    RoundedRectangle::RoundedRectangle():
        __radius(0),
        __size(100, 50),
        __pointCount(30),
        __pointEachCount(1.0f*__pointCount/4) {
        update();
    };
    RoundedRectangle::~RoundedRectangle() {
    }
    
    bool RoundedRectangle::contains(const sf::Vector2f& point) const {
        sf::FloatRect rect = Shape::getGlobalBounds();
        rect.left+=__radius;
        rect.width-=2*__radius;
        if (rect.contains(point)) return true;
        rect.left-=__radius;
        rect.width+=2*__radius;
        rect.top+=__radius;
        rect.height-=2*__radius;
        if (rect.contains(point)) return true;
        rect.left+=__radius;
        rect.width-=2*__radius;
        sf::Vector2f vec(rect.left-point.x, rect.top-point.y);
        if (vec.x*vec.x + vec.y*vec.y<=__radius*__radius) return true;
        if (vec+={rect.width,0}; vec.x*vec.x+vec.y*vec.y <= __radius*__radius) return true;
        if (vec+={0, rect.height}; vec.x*vec.x + vec.y*vec.y<=__radius*__radius) return true;
        if (vec-={rect.width, 0}; vec.x*vec.x + vec.y*vec.y<=__radius*__radius) return true;
        return false;
    }
    
    std::size_t RoundedRectangle::getPointCount() const {
        return __pointCount;
    }
    sf::Vector2f RoundedRectangle::getPoint(size_t index) const {
        if (!index) return {0, __radius};
        else if (index < __pointEachCount - 1) {
            float angle = M_PI/2*index/__pointEachCount;
            return sf::Vector2f(__radius - __radius*cos(angle), __radius - __radius*sin(angle));
        }
        else if (index < __pointEachCount) return {__radius, 0};
        else if (index < __pointEachCount+1) return {__size.x - __radius, 0};
        else if (index < 2*__pointEachCount-1) {
            float angle = M_PI/2*index/__pointEachCount;
            return sf::Vector2f(__size.x - __radius - __radius*cos(angle), __radius - __radius*sin(angle));
        }
        else if (index < 2*__pointEachCount) return {__size.x,__radius};
        else if (index < 2*__pointEachCount+1) return {__size.x, __size.y-__radius};
        else if (index < 3*__pointEachCount-1) {
            float angle = M_PI/2*index/__pointEachCount;
            return sf::Vector2f(__size.x - __radius - __radius*cos(angle), __size.y - __radius - __radius*sin(angle));
        }
        else if (index < 3*__pointEachCount) return {__size.x - __radius, __size.y};
        else if (index < 3*__pointEachCount+1) return {__radius, __size.y};
        else if (index < __pointCount-1) {
            float angle = M_PI/2*index/__pointEachCount;
            return sf::Vector2f(__radius - __radius*cos(angle), __size.y - __radius - __radius*sin(angle));
        }
        else return {0, __size.y - __radius};
    }
    void RoundedRectangle::setPointCount(const size_t& count) {
        __pointCount = count;
        __pointEachCount = 1.0f*__pointCount/4;
        update();
    }
    
    void RoundedRectangle::setSize(const float& x, const float& y) {
        __size = {x, y};
        update();
    }
    void RoundedRectangle::update() {
        Shape::update();
    }
    void RoundedRectangle::setRoundness(const float& radius) {
        __radius = radius;
        update();
    }
    sf::Vector2f RoundedRectangle::getCenter() const {
        return Shape::getPosition()+__size/2.f;
    }
    sf::Vector2f RoundedRectangle::getSize() const {
        return __size;
    }
    
}