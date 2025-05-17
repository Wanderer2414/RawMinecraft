#ifndef CAMERA_H
#define CAMERA_H
#include "Controller.h"
#include "Global.h"

class Camera: public Controller {
public:
    Camera();
    ~Camera();

    virtual void    setPosition(const float& x, const float& y, const float& z),
                    move(const float& x, const float& y, const float& z),
                    rotate(const float& vertical_angle, const float& horizontal_angle),
                    setCenter(const float& x, const float& y, const float& z),
                    setWide(const float& angle),
                    setNearProjection(const float& near),
                    setFarProjection(const float& far);
    Vector3f        getHorizontalVector() const,
                    getCenter() const;
protected:
    virtual         handle_function(handle) override;
private:
    bool            pOnGround, pUpward;
    Vector2i        pWindowCenter;
    Vector3f        pPosition, pCenter, pDelta;
    float           pAngle, pNear, pFar, pSpeed, pJumpHeight;
};
#endif