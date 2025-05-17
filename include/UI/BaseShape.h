#ifndef BASE_SHAPE_H
#define BASE_SHAPE_H
#include "Global.h"
class BaseShape: public Shape {
public:
    virtual bool        contains(const Vector2f& point)     const = 0;
    virtual Vector2f    getCenter()                         const = 0,
                        getSize()                           const = 0;
    virtual void        update()                                    ;
};
#endif