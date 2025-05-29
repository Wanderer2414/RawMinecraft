#ifndef CIRCLE_H
#define CIRCLE_H
#include "BaseShape.h"
namespace MyBase {
    class Circle: public BaseShape, public sf::Shape {
    public:
        Circle();
        ~Circle();
        virtual bool        contains(const sf::Vector2f& point) const override;
    
        virtual size_t      getPointCount() const override;
    
        virtual void        setPointCount(const size_t& pointCount),
                            setRadius(const float& radius);
    
        virtual sf::Vector2f getPoint(size_t index) const override,
                            getCenter()             const override,
                            getSize()               const override;
        virtual void        update()                override;
    private:
        float           __radius;
        std::size_t     __pointCount;
    };
};
#endif