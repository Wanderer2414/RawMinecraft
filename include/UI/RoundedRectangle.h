#ifndef ROUNDEDRECTANGLE_H
#define ROUNDEDRECTANGLE_H
#include "BaseShape.h"

class RoundedRectangle:public BaseShape {
public:
    RoundedRectangle();

    virtual bool contains(const Vector2f& point) const override;

    size_t      getPointCount() const override;

    float       getRadius() const;

    Vector2f    getSize() const override,
                getPoint(size_t index)  const override,
                getCenter()             const override;

    virtual void setSize(const float& x, const float& y),
                setRoundness(const float& radius),
                setPointCount(const size_t& count);
protected:
private:
    Vector2f    m_size;
    size_t      point_count;
    float       m_radius, 
                pointEachCount;
};

#endif