#include "Form3D.h"
#include "Container.h"
#include "Controller.h"
#include "General.h"
#include "Global.h"
#include "spriv_extended.h"

namespace MyBase3D {

    Form3D::Form3D(const int& index): __formIndex(index), __returnValue(INT_MIN) {
        __sensitiveClock.setDuration(30);
    }
    Form3D::~Form3D() {}
    bool Form3D::contains(const glm::vec2& position) const {
        return true;
    }
    bool Form3D::handle(GLFWwindow* window) {
        bool is_changed = MyBase::Container::handle(window);
        is_changed = _camera.handle(window) || is_changed;
        return is_changed;
    }
    bool Form3D::catchEvent(GLFWwindow* window) {
        bool is_changed = MyBase::Container::catchEvent(window);   
        is_changed = _camera.catchEvent(window) || is_changed;
        return is_changed;
    }
    bool Form3D::sensitiveHandle(GLFWwindow* window) {
        bool is_changed = MyBase::Container::sensitiveHandle(window);
        is_changed = _camera.sensitiveHandle(window) || is_changed;
        return is_changed;
    }
    int Form3D::run(GLFWwindow* window) {
        bool is_changed = true, is_catched = false;
        while (!glfwWindowShouldClose(window)) {
            Container::reset();
            glfwPollEvents();
            is_changed = catchEvent(window) || is_changed;
            if (__sensitiveClock.get()) {
                __sensitiveClock.restart();
                is_changed = sensitiveHandle(window) || is_changed;
            }
            is_catched = Container::setHover(getMousePosition(window)) || is_catched;
            is_changed = handle(window) || is_changed;
            if (is_changed) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0, 0, 0, 0);
                glDraw();
                glfwSwapBuffers(window);
            }
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }
            if (__returnValue!=INT_MIN) return __returnValue;
            is_changed = 0;
        }
        return __formIndex;
    }
    int Form3D::getReturnForm() const {
        return __returnValue;
    }
    int Form3D::getFormIndex() {
        return __formIndex;
    }
    void Form3D::setReturnForm(const int& returnValue) {
        __returnValue = returnValue;
    }
    void Form3D::setSensitiveTime(const size_t& time) {
        __sensitiveClock.setDuration(time);
    }
    void Form3D::glDraw() const {
        _camera.glDraw();
        Container::glDraw();
    }
}