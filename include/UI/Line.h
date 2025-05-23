#ifndef LINE_H
#define LINE_H
#include "Global.h"
class Line {
public:
    Line(const Vector3f& origin,  const Vector3f& delta);
    ~Line();

    virtual bool    contains(const Vector3f& position) const;
    virtual float   distance(const Vector3f& position) const;

protected:
    Vector3f    sOrigin, sDelta;
};
#endif