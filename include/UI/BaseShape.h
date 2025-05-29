#ifndef BASE_SHAPE_H
#define BASE_SHAPE_H
#include "Global.h"
namespace MyBase {
    class BaseShape {
        public:
            BaseShape()                                                   = default;
            ~BaseShape()                                                  = default;
            virtual bool            contains(const sf::Vector2f& point)     const = 0;
            virtual sf::Vector2f    getCenter()                             const = 0,
                                    getSize()                               const = 0;
            virtual void            update()                                      = 0;
    };
}
#endif