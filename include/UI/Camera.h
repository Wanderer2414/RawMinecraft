#ifndef CAMERA_H
#define CAMERA_H
#include "Alarm.h"
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
    virtual void    draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
private:
    bool            pOnGround, pUpward;
    Vector2i        pWindowCenter;
    Vector3f        pPosition, pDelta;
    double          pVerticalAngle, pHorizontalAngle;
    float           pAngle, pNear, pFar, pSpeed, pJumpHeight, pDistance;
    Alarm           pFrameAlarm;

    Vector2f        transfer(const Vector3f& vector) const;
};
#endif