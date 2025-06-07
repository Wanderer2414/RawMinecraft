#ifndef MODEL_H
#define MODEL_H
#include "Clock.h"
#include "Controller3D.h"
#include "GLFW/glfw3.h"
namespace MyCraft {
    class Model: public MyBase3D::Controller3D {
        public:
            Model();
            ~Model();
            void glDraw() const override;
            bool handle(GLFWwindow* window) override;
        private:
            MyBase::Clock   __clock;
            float           __sec;
    };
}
#endif