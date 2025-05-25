#ifndef CAMERA_H
#define CAMERA_H
#include "Alarm.h"
#include "Controller3D.h"
#include "Global.h"
#include "Ray.h"
#include "World.h"
class Form3D;
class Camera: public Controller3D {
public:
    Camera();
    ~Camera();

    virtual void    move(const float& x, const float& y, const float& z),
                    rotate(const float& vertical_angle, const float& horizontal_angle),
                    setPosition(const glm::vec3& position),
                    setCameraDirection(const glm::vec3& position, const glm::vec3& center),
                    setPerpective(const float& angle, const float& aspect, const float& near, const float& far);
    glm::vec3       getHorizontalVector() const,
                    getCenter() const,
                    getDirection() const,
                    getPosition() const;

    Ray3f           getSight() const;
    operator GLuint();
    friend class    Form3D;
protected:
    virtual         handle_function(handle) override;
    virtual void    draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
    virtual void    update() override;
private:
    Vector2i        pWindowCenter;
    glm::vec3       pPosition, pDelta;
    double          pVerticalAngle;
    float           pDistance;
    VertexArray     pDirection;
    glm::mat4       pView, pProjection, pClipPlane;

    GLuint          pCamera;
    Vector2f        transfer(const glm::vec3& vector) const;
};
#endif