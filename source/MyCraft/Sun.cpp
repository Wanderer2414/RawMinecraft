#include "Sun.h"
#include "Camera.h"
#include "ControlCenter.h"
#include "Global.h"
#include "Message.h"
#include "Shape.h"
#include "ShapeManager.h"
#include "Texture.h"

namespace MyCraft {
    Sun::Sun(): __time(0), __offset(0,0) {
        glGenBuffers(1,&__lightBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, __lightBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(float)*2, 0, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 10, __lightBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, __lightBuffer);
        update();

        MyBase::Texture texture = MyBase::Texture("assets/images/sky.png");
        setTexture(texture);
        setTextureImportPosition({0,0});
        setTextureImportSize({1,1});
        setTextureExportPosition({-1,-1});
        setTextureExportSize({2, 2});

        MyBase::ShapeManager::getInstance().createShape(__coverSky, {2,2});
        __skyContainer.setFillColor({0,0,0,200});
        __skyContainer.setPosition({-1,-1});

        __sunContainer.setFillColor(WHITE);
        __sunContainer.setPosition({0, -1.5});
        MyBase::ShapeManager::getInstance().createShape(__sun, {0.1/MyBase::ControlCenter::getInstance().GetWindowRatio(),0.1});

        __clock.setDuration(100);
        add(new SunMoveCommand(*this));
    }
    Sun::~Sun() {
        glDeleteBuffers(1, &__lightBuffer);
        MyBase::ShapeManager::getInstance().removeShape(__coverSky, {2,2});
        MyBase::ShapeManager::getInstance().removeShape(__sun, {0.1/MyBase::ControlCenter::getInstance().GetWindowRatio(),0.1});
    }

    void Sun::update() {
        glBindBuffer(GL_UNIFORM_BUFFER, __lightBuffer);
        float light[2] = {-0.7, 0.7f*(1-2*abs(0.5f-__time))};
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float)*2, &light[0]);
        __skyContainer.setFillColor({0,0,0, (unsigned char)(255.f*2*abs(__time-0.5f))});

        float angle = 2*M_PI*__time - M_PI_2;
        glm::vec3 position = glm::vec3(__offset.x+100*cos(angle),__offset.y - (__offset.y)/1000,100*sin(angle));
        glm::vec2 pos2D = MyBase3D::Camera::transfer(position);
        __sunContainer.setPosition(pos2D - __sun.getSize()/2.f);
    }
    bool Sun::handle(GLFWwindow* window) {
        if (__clock.get()) {
            __clock.restart();
            __time+=0.001;
            if (__time>1) __time-=1;
            update();
            return true;
        }
        return false;
    }
    void Sun::glDraw() const {
        MyBase::TextureContainer::draw();
        __skyContainer.draw(__coverSky);
        __sunContainer.draw(__sun);
    }

    SunMoveCommand::SunMoveCommand(Sun& sun): __sun(sun) {}
    SunMoveCommand::~SunMoveCommand() {}

    MyBase::MessageType SunMoveCommand::getType() const {
        return MyBase::SetCamera;
    }
    void SunMoveCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        MyBase::SetCameraMessage* package = (MyBase::SetCameraMessage*)message;
        __sun.__offset = package->position;
        __sun.update();
    }
}