#ifndef CAMERA_H
#define CAMERA_H
#include "Global.h"
class Camera {
public:
    Camera();
    ~Camera();

    virtual void    setPosition(const float& x, const float& y, const float& z),
                    move(const float& x, const float& y, const float& z),
                    setCenter(const float& x, const float& y, const float& z),
                    setWide(const float& angle),
                    setNearProjection(const float& near),
                    setFarProjection(const float& far);
    Vector3f        getHorizontalVector() const,
                    getCenter() const;
private:
    Vector3f        pPosition, pCenter;
    float           pAngle, pNear, pFar;
};
#endif