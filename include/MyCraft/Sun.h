#ifndef SUN_H
#define SUN_H
#include "Clock.h"
#include "Container3D.h"
#include "Controller3D.h"
#include "Ellipse.h"
#include "Message.h"
#include "Rectangle.h"
#include "Shape.h"
#include "Texture.h"
namespace MyCraft {
    class Sun: private MyBase::TextureContainer, public MyBase::Port {
    public:
        Sun();
        ~Sun();

        void update();
        bool handle(GLFWwindow* window);
        void glDraw() const            ;
        friend class SunMoveCommand;
    private:    
        glm::vec2                   __offset;
        float                       __time;
        MyBase::Clock               __clock;
        GLuint                      __lightBuffer;
        MyBase::Rectangle           __coverSky;
        MyBase::Ellipse             __sun;
        MyBase::ShapeContainer      __skyContainer, __sunContainer;
    };

    class SunMoveCommand: public MyBase::Command {
    public:
        SunMoveCommand(Sun& sun);
        ~SunMoveCommand();

        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        Sun& __sun;
    };
}
#endif