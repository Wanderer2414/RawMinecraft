#ifndef CAMERA_H
#define CAMERA_H
#include "Controller3D.h"
#include "Global.h"
#include "Message.h"
#include "Ray.h"
namespace MyBase3D {
    class Form3D;
    #define CAMERA_DISTANCE 4.0f
    class Camera: public MyBase3D::Controller3D, public MyCraft::Port {
    public:
        Camera();
        ~Camera();

        virtual void    move(const glm::vec3& delta),
                        rotate(const float& vertical_angle, const float& horizontal_angle),
                        look(const glm::vec3& pos),

                        setPosition(const glm::vec3& position),
                        setPosition(const float& x, const float& y, const float& z),

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
        virtual void    glDraw() const override;
        virtual void    update() override;
    private:
        glm::vec2       __windowCenter;
        glm::vec3       __position, __delta;
        double          __verticalAngle;
        glm::vec2       __direction[6];
        glm::vec4       __direction_color[6];
        glm::mat4       __view, 
                        __projection, 
                        __clipPlane;

        GLuint          __camera;
        glm::vec2       transfer(const glm::vec3& vector) const;
    };
};
namespace MyCraft {
    class ThirdCameraSetup: public MyCraft::Command {
    public:
        ThirdCameraSetup(MyBase3D::Camera* camera);
        ~ThirdCameraSetup();
        MessageType getType() const override;
        void execute(Port& mine, Port& source, Message* message) override;
    private:
        MyBase3D::Camera* __camera;
    };
}
#endif