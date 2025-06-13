#include "Form3D.h"
#include "Controller.h"
#include "GLFW/glfw3.h"
#include "General.h"
#include "Global.h"
#include "spriv_extended.h"

namespace MyBase3D {

    Form3D::Form3D(const int& index) {
        _formIndex = index;
        _returnValue = INT_MIN;
    }
    Form3D::~Form3D() {
    }
    bool Form3D::contains(const glm::vec2& position) const {
        return true;
    }
    bool Form3D::handle(GLFWwindow* window) {
        bool is_changed = false;
        is_changed = _camera.handle(window) || is_changed;
        is_changed = Container::handle(window) || is_changed;
        return is_changed;
    }
    int Form3D::run(GLFWwindow* window) {
        bool is_changed = true, is_catched = false;
        while (!glfwWindowShouldClose(window)) {
            Container::reset();
            glfwPollEvents();
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
            if (_returnValue!=INT_MIN) return _returnValue;
            is_changed = 0;
        }
        return _formIndex;
    }
    
    void Form3D::glDraw() const {
        _camera.glDraw();
        Container::glDraw();
    }
}