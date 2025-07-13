#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "Shape.h"
namespace MyBase {
    class Ellipse: public Shape {
    public:
        Ellipse();
        Ellipse(const glm::vec2& size);
        ~Ellipse();

        bool        contains(const glm::vec2& position) const override;
        size_t      getPointCount() const override;
        glm::vec2   getPoint(const size_t& index) const override;
        glm::vec2   getCenter() const override,
                    getSize() const;
    private:
        static const unsigned int pointCount = 30;
        glm::vec2 __size;
    };
}
#endif