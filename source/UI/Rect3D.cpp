#include "Rect3D.h"
#include "General.h"
#include "Ray.h"
#include "Segment.h"
#include "glm/geometric.hpp"
namespace MyBase3D {

    Rect3f::Rect3f(): __A(0,0,0), __B(0,0,0), __C(0,0,0) {
    }
    Rect3f::Rect3f(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
        __A = a; __B = b; __C = c;
    }
    Rect3f::~Rect3f() {
    
    }
    bool Rect3f::isIntersect(const Ray3f& ray) const {
        glm::vec3 normal = det(__B-__A, __C-__B);
        if (ray*normal == 0) return false;
        float normalSide = ray*normal/glm::length(normal);
        float times = std::abs(distance(ray.getOrigin())/normalSide);
        glm::vec3 pos = ray.getOrigin() + times*(glm::vec3)ray;
        return contains(pos);
    }
    bool Rect3f::contains(const glm::vec3& position) const {
        Segment AB(__A, __B);
        Segment BC(__B, __C);
        Segment CD(__C, operator[](3));
        Segment DA(operator[](3), __A);
        if (std::abs(AB.distance(position)+CD.distance(position) - BC.length())<0.01 && std::abs(BC.distance(position) + DA.distance(position) - AB.length())<0.01)
            return true;
        return false;
    }
    float Rect3f::distance(const glm::vec3& position) const {
        glm::vec3 delta = position-__A;
        glm::vec3 normal = det(__B-__A, __C-__B);
        return delta*normal/glm::length(normal);
    }
    glm::vec3 Rect3f::operator[](const unsigned int& index) const {
        switch (index) {
        case 0: return __A;
        case 1: return __B;
        case 2: return __C;
        default: return __C+__A-__B;
        }
    }
    void Rect3f::move(const glm::vec3& offset) {
        __A += offset;
        __B += offset;
        __C += offset;
    }
    glm::vec3 Rect3f::getCenter() const {
        return (__A+__C)/2.f;
    }
    glm::vec3 Rect3f::getNormal() const {
        return det(__B-__A, __C-__B);
    }
    glm::vec3 Rect3f::getIntersect(const Ray3f& ray) const {
        glm::vec3 normal = det(__B-__A, __C-__B);
        float normalSide = ray*normal/glm::length(normal);
        float times = std::abs(distance(ray.getOrigin())/normalSide);
        glm::vec3 pos = ray.getOrigin() + times*(glm::vec3)ray;
        return pos;
    }
}