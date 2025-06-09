#ifndef MODEL_H
#define MODEL_H
#include "Clock.h"
#include "Controller3D.h"
#include "GLFW/glfw3.h"
namespace MyCraft {
    class Model: public MyBase3D::Controller3D {
        public:
            virtual void    see(const glm::vec3& dir) = 0,
                            move(const glm::vec3& dir) = 0,
                            rotate(const float& angle) = 0;
        private:
            
    };
}
#endif