#ifndef CAMERA_H
#define CAMERA_H
#include "Clock.h"
#include "Controller.h"
#include "Global.h"
#include "Message.h"
#include "Ray.h"
namespace MyBase3D {
    class Form3D;
    #define CAMERA_DISTANCE 4.0f
    class Camera: public MyBase::Controller, public MyCraft::Port {
    public:
        Camera();
        ~Camera();

        virtual void    move(const glm::vec3& delta),
                        rotate(const float& vertical_angle, const float& horizontal_angle),
                        look(const glm::vec3& pos),
                        see(const glm::vec3& dir),

                        setPosition(const glm::vec3& position),
                        setPosition(const float& x, const float& y, const float& z),

                        setCameraDirection(const glm::vec3& position, const glm::vec3& center),
                        setPerpective(const float& angle, const float& aspect, const float& near, const float& far);

        glm::vec3       getHorizontalVector() const,
                        getCenter() const,
                        getDirection() const,
                        getCameraPosition() const;

        Ray3f           getSight() const;
        operator GLuint();
        friend class    Form3D;
    protected:
        virtual bool    handle(GLFWwindow* window) override;
        virtual void    glDraw() const override;
        virtual void    update() override;
    private:
        bool            __isThirdCamera;
        glm::vec2       __windowCenter;
        glm::vec3       __position, __delta;
        double          __verticalAngle;
        glm::vec2       __direction[6];
        glm::vec4       __direction_color[6];
        glm::mat4       __view, 
                        __projection, 
                        __clipPlane;

        GLuint          __camera;
        MyBase::Clock   __keyCooldown;
        glm::vec2       transfer(const glm::vec3& vector) const;
    };
};
namespace MyCraft {
    class MoveCameraMessage: public MyCraft::Message {
    public:
        MoveCameraMessage(const glm::vec3& direction);
        ~MoveCameraMessage();
        MessageType getType() const override;
        glm::vec3 direction;
    };
    class RotateCameraMessage: public MyCraft::Message {
    public:
        RotateCameraMessage(const glm::vec3& position, const glm::vec3& direction);
        ~RotateCameraMessage();
        MessageType getType() const override;
        glm::vec3 direction;
        glm::vec3 position;
    };
    class MoveCameraCommand: public MyCraft::Command {
    public:
        MoveCameraCommand(MyBase3D::Camera* camera);
        ~MoveCameraCommand();
        MessageType getType() const override;
        void execute(Port& mine, Port& source, Message* message) override;
    private:
        MyBase3D::Camera* __camera;
    };
    class RotateCameraCommand_ThirdPersonView: public MyCraft::Command {
    public:
        RotateCameraCommand_ThirdPersonView(MyBase3D::Camera* camera);
        ~RotateCameraCommand_ThirdPersonView();
        MessageType getType() const override;
        void execute(Port& mine, Port& source, Message* message) override;
    private:
        MyBase3D::Camera* __camera;
    };    
    class RotateCameraCommand_FirstPersonView: public MyCraft::Command {
    public:
        RotateCameraCommand_FirstPersonView(MyBase3D::Camera* camera);
        ~RotateCameraCommand_FirstPersonView();
        MessageType getType() const override;
        void execute(Port& mine, Port& source, Message* message) override;
    private:
        MyBase3D::Camera* __camera;
    };    

    class ResetCameraMessage: public Message {
    public:
        ResetCameraMessage(const bool& isFirstCamera);
        MessageType getType() const override;
        bool isFirstCamera;
    private:
    };
}
#endif