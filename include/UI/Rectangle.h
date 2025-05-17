#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "BaseShape.h"

class Rectangle:public BaseShape, public Shape {
public:
    Rectangle();

    virtual bool contains(const Vector2f& point) const override;

    size_t      getPointCount() const override;

    Vector2f    getSize() const override,
                getPoint(size_t index)  const override,
                getCenter()             const override;

    virtual void setSize(const float& x, const float& y);
    virtual void update() override;
protected:
private:
    Vector2f    m_size;
};

#endif