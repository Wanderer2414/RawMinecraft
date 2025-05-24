#ifndef CAMERA_H
#define CAMERA_H
#include "Alarm.h"
#include "Controller3D.h"
#include "Global.h"
#include "Ray.h"
class Form3D;
class Camera: public Controller3D {
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
    glm::vec3        getHorizontalVector() const,
                    getCenter() const;

    Ray3f           getSight() const;
    operator GLuint();
    friend class    Form3D;
protected:
    virtual         handle_function(handle) override;
    virtual void    draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
    virtual void    update() override;
private:
    bool            pOnGround, pUpward;
    Vector2i        pWindowCenter;
    glm::vec3        pPosition, pDelta;
    double          pVerticalAngle, pHorizontalAngle;
    float           pAngle, pNear, pFar, pSpeed, pJumpHeight, pDistance;
    Alarm           pFrameAlarm;
    VertexArray     pDirection;
    glm::mat4       pView;

    GLuint          pCamera;
    Vector2f        transfer(const glm::vec3& vector) const;
};
#endif