#include "Sun.h"
#include "Global.h"

namespace MyCraft {
    Sun::Sun(): __time(0) {
        glGenBuffers(1,&__lightBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, __lightBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(float)*2, 0, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 10, __lightBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, __lightBuffer);
        update();
    }
    Sun::~Sun() {
        glDeleteBuffers(1, &__lightBuffer);
    }

    void Sun::update() {
        glBindBuffer(GL_UNIFORM_BUFFER, __lightBuffer);
        float light[2] = {-0.35, 0};
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float)*2, &light[0]);
    }
    bool Sun::handle(GLFWwindow* window) {
        return false;
    }
}