#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "BaseShape.h"
namespace MyBase {
    class Rectangle:public BaseShape, public sf::Shape {
    public:
        Rectangle();
        ~Rectangle();

        virtual bool        contains(const sf::Vector2f& point)     const override;

        std::size_t         getPointCount()                         const override;

        sf::Vector2f        getSize()                               const override,
                            getPoint(std::size_t index)             const override,
                            getCenter()                             const override;

        virtual void        setSize(const float& x, const float& y),
                            setSize(const sf::Vector2f& size),
                            update() override;
    protected:
    private:
        sf::Vector2f    __size;
    };
}
#endif