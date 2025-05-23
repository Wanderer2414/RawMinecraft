#ifndef TRACK_RECT_H
#define TRACK_RECT_H
#include "Controller3D.h"
#include "Ray.h"
#include "Rect3D.h"
class TrackRect: public Controller3D {
public:
    TrackRect();
    ~TrackRect();

    virtual bool setHover(const Ray3f& sight) override;

    virtual void setPosition(const Vector3f& a, const Vector3f& b, const Vector3f& c);
protected:
    virtual void draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
private:
    Rect3f pRect;
    Vector3f position;
};
#endif