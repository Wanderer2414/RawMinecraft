#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "BaseShape.h"

class Rectangle:public BaseShape {
public:
    Rectangle();

    virtual bool contains(const Vector2f& point) const override;

    size_t      getPointCount() const override;

    Vector2f    getSize() const,
                getPoint(size_t index)  const override,
                getCenter()             const override;

    virtual void setSize(const float& x, const float& y);
protected:
private:
    Vector2f    m_size;
};

#endif