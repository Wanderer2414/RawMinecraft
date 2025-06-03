#include "Form.h"
#include "Container.h"
#include "Controller.h"
#include "GLFW/glfw3.h"
#include "Global.h"
#include <GL/gl.h>

namespace MyBase {
    Form::Form(const int& index) {
        _formIndex = index;
        _returnValue = INT_MIN;
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
            if (is_changed) {
                glClearColor(0, 0, 0, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glDraw();
                glfwSwapBuffers(window);
            }
            if (_returnValue!=INT_MIN) return _returnValue;
            is_changed = 0;
        }
        return _formIndex;
    }
    void Form::glDraw() const {
        for (const auto& [child, layer]:children) 
            if (_currentFocus == -1 || child != children[_currentFocus].first) child->glDraw();
        if (_currentFocus != -1) glDraw();
    }
}