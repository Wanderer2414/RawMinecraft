#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "Shape.h"
namespace MyBase {
    class Rectangle: public Shape {
    public: 
        Rectangle();
        Rectangle(const glm::vec2& size);
        ~Rectangle();
        bool contains(const glm::vec2& position) const override;
        size_t getPointCount() const override;
        glm::vec2   getPoint(const size_t& index) const override,
                    getSize() const,
                    getCenter() const override;
    private:
        glm::vec2 __size;
    };
}
#endif