#ifndef SUN_H
#define SUN_H
#include "Clock.h"
#include "Controller2D.h"
#include "Controller3D.h"
namespace MyCraft {
    class Sun: public MyBase3D::Controller3D {
    public:
        Sun();
        ~Sun();

        void update() override;
    private:    
        int                         __time;
        MyBase::Clock               __clock;
        GLuint                      __lightBuffer;

        bool handle(GLFWwindow* window) override;
    };
}
#endif