#include "Form.h"
#include "Container.h"
#include "Controller.h"
#include "GLFW/glfw3.h"
#include "Global.h"
#include <GL/gl.h>

namespace MyBase {
    Form::Form(const int& index): __formIndex(index), __returnValue(INT_MIN) {
        __sensitiveClock.setDuration(10);
    }
    Form::~Form() {
    
    }
    bool Form::contains(const glm::vec2& position) const {
        return true;
    }
    int Form::run(GLFWwindow* window) {
        bool is_changed = true;
        while (glfwWindowShouldClose(window)) {
            reset();
            glfwPollEvents();
            is_changed = handle(window) || is_changed;
            if (__sensitiveClock.get()) {
                __sensitiveClock.restart();
                is_changed = catchEvent(window) || is_changed;
                is_changed = sensitiveHandle(window) || is_changed;
            }
            if (is_changed) {
                glClearColor(0, 0, 0, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glDraw();
                glfwSwapBuffers(window);
            }
            if (__returnValue!=INT_MIN) return __returnValue;
            is_changed = 0;
        }
        return __formIndex;
    }
    void Form::setSensitiveTime(const size_t& time) {
        __sensitiveClock.setDuration(time);
    }
}