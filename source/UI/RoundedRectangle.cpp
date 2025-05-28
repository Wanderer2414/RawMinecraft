#include "RoundedRectangle.h"
#include "BaseShape.h"
#include "Global.h"
namespace MyBase {

    RoundedRectangle::RoundedRectangle() {
        m_radius = 0;
        m_size = {100, 50};
        point_count = 30;
        pointEachCount = 1.0f*point_count/4;
        update();
    };
    
    
    bool RoundedRectangle::contains(const sf::Vector2f& point) const {
        sf::FloatRect rect = Shape::getGlobalBounds();
        rect.left+=m_radius;
        rect.width-=2*m_radius;
        if (rect.contains(point)) return true;
        rect.left-=m_radius;
        rect.width+=2*m_radius;
        rect.top+=m_radius;
        rect.height-=2*m_radius;
        if (rect.contains(point)) return true;
        rect.left+=m_radius;
        rect.width-=2*m_radius;
        sf::Vector2f vec(rect.left-point.x, rect.top-point.y);
        if (vec.x*vec.x + vec.y*vec.y<=m_radius*m_radius) return true;
        if (vec+={rect.width,0}; vec.x*vec.x+vec.y*vec.y <= m_radius*m_radius) return true;
        if (vec+={0, rect.height}; vec.x*vec.x + vec.y*vec.y<=m_radius*m_radius) return true;
        if (vec-={rect.width, 0}; vec.x*vec.x + vec.y*vec.y<=m_radius*m_radius) return true;
        return false;
    }
    
    size_t RoundedRectangle::getPointCount() const {
        return point_count;
    }
    sf::Vector2f RoundedRectangle::getPoint(size_t index) const {
        if (!index) return {0, m_radius};
        else if (index < pointEachCount - 1) {
            float angle = M_PI/2*index/pointEachCount;
            return sf::Vector2f(m_radius - m_radius*cos(angle), m_radius - m_radius*sin(angle));
        }
        else if (index < pointEachCount) return {m_radius, 0};
        else if (index < pointEachCount+1) return {m_size.x - m_radius, 0};
        else if (index < 2*pointEachCount-1) {
            float angle = M_PI/2*index/pointEachCount;
            return sf::Vector2f(m_size.x - m_radius - m_radius*cos(angle), m_radius - m_radius*sin(angle));
        }
        else if (index < 2*pointEachCount) return {m_size.x,m_radius};
        else if (index < 2*pointEachCount+1) return {m_size.x, m_size.y-m_radius};
        else if (index < 3*pointEachCount-1) {
            float angle = M_PI/2*index/pointEachCount;
            return sf::Vector2f(m_size.x - m_radius - m_radius*cos(angle), m_size.y - m_radius - m_radius*sin(angle));
        }
        else if (index < 3*pointEachCount) return {m_size.x - m_radius, m_size.y};
        else if (index < 3*pointEachCount+1) return {m_radius, m_size.y};
        else if (index < point_count-1) {
            float angle = M_PI/2*index/pointEachCount;
            return sf::Vector2f(m_radius - m_radius*cos(angle), m_size.y - m_radius - m_radius*sin(angle));
        }
        else return {0, m_size.y - m_radius};
    }
    void RoundedRectangle::setPointCount(const size_t& count) {
        point_count = count;
        pointEachCount = 1.0f*point_count/4;
        update();
    }
    
    void RoundedRectangle::setSize(const float& x, const float& y) {
        m_size = {x, y};
        update();
    }
    void RoundedRectangle::update() {
        Shape::update();
    }
    void RoundedRectangle::setRoundness(const float& radius) {
        m_radius = radius;
        update();
    }
    Vector2f RoundedRectangle::getCenter() const {
        return Shape::getPosition()+m_size/2.f;
    }
    Vector2f RoundedRectangle::getSize() const {
        return m_size;
    }
    
}