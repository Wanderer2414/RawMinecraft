#ifndef CIRCLE_H
#define CIRCLE_H
#include "BaseShape.h"
class Circle: public BaseShape {
public:
    Circle();
    ~Circle();
    virtual bool    contains(const Vector2f& point) const override;

    virtual size_t  getPointCount() const override;

    virtual void    setPointCount(const size_t& pointCount),
                    setRadius(const float& radius);

    virtual Vector2f getPoint(size_t index) const override,
                    getCenter()             const override,
                    getSize()               const override;
protected:
    float           m_radius;
private:
    size_t          m_point_count;
};
#endif