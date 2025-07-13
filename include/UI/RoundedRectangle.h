#ifndef ROUNDEDRECTANGLE_H
#define ROUNDEDRECTANGLE_H
#include "Shape.h"
namespace MyBase {

    class RoundedRectangle:public Shape {
        public:
            RoundedRectangle();
            RoundedRectangle(const glm::vec2& size, const float& radius);
            bool contains(const glm::vec2& point) const override;
        
            float       getRadius() const;
            size_t      getPointCount() const override;
            glm::vec2   getPoint(const size_t& index)  const override;
            glm::vec2   getSize() const,
                        getCenter() const override;
        protected:
        private:
            unsigned char __pointEachCount;
            glm::vec2   __size;
            float       __radius;
        };
        
}
#endif