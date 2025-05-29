#ifndef ROUNDEDRECTANGLE_H
#define ROUNDEDRECTANGLE_H
#include "BaseShape.h"
namespace MyBase {

class RoundedRectangle:public BaseShape, public sf::Shape {
    public:
        RoundedRectangle();
        ~RoundedRectangle();

        virtual bool    contains(const sf::Vector2f& point) const override;
    
        size_t          getPointCount() const override;
    
        float           getRadius() const;
    
        sf::Vector2f    getSize()               const override,
                        getPoint(size_t index)  const override,
                        getCenter()             const override;

        virtual void    setSize(const float& x, const float& y),
                        setRoundness(const float& radius),
                        setPointCount(const size_t& count);
        virtual void    update() override;
    protected:
    private:
        sf::Vector2f    __size;
        size_t          __pointCount;
        float           __radius, 
                        __pointEachCount;
    };
}

#endif