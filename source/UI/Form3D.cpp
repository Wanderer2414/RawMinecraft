#include "Form3D.h"
#include "Camera.h"
#include "Color.h"
#include "Container2D.h"
#include "Container3D.h"
#include "ControlCenter.h"
#include "Global.h"
#include "Shape.h"
#include "ShapeManager.h"
#include <ctime>

namespace MyBase3D {
    Form3D::Form3D(const int& index): __formIndex(index), __returnValue(-1), __isOpen(false), __backgroundColor(WHITE), __frameCount(0), __deltaClock(1) {
        insert(&Camera::Instance());
    }
    Form3D::~Form3D() {
        Camera::close();
    }
    bool Form3D::contains(const glm::vec2& position) const {
        return true;
    }
    float Form3D::getAverageFps() const {
        return 1.0f*(CLOCKS_PER_SEC)*__frameCount/(clock()-__startClock);
    }
    float Form3D::getCurrentFps() const {
        if (__deltaClock==0) return 100000;
        return 1.0f*CLOCKS_PER_SEC/__deltaClock;
    }
    void Form3D::__open(GLFWwindow* window) {}
    void Form3D::__close(GLFWwindow* window) {}
    int Form3D::run(GLFWwindow* window) {
        __currentClock = __startClock = clock();
        bool is_changed = true;
        __isOpen = true;
        __open(window);
        while (!glfwWindowShouldClose(window) && __isOpen) {
            MyBase::ControlCenter::getInstance().Reset();
            Container2D::reset();
            Container3D::reset();
            glfwPollEvents();
            is_changed = setHover(MyBase::ControlCenter::getInstance().getCursorPos(window)) || is_changed;
            is_changed = catchEvent(window) || is_changed;
            is_changed = handle(window) || is_changed;
            if (is_changed) {
                glm::vec4 color = __backgroundColor.getColor();
                glClearColor(color.r, color.g, color.b, color.a);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glDraw();
                glDrawTransparent();
                glfwSwapBuffers(window);
            }
            is_changed = 0;
            __deltaClock = clock() - __currentClock;
            __currentClock = clock();
            __frameCount++;
        }
        __close(window);
        return __returnValue;
    }
    bool Form3D::catchEvent(GLFWwindow* window) {
        bool is_changed = false;
        is_changed = Container2D::catchEvent(window) || is_changed;
        is_changed = Container3D::catchEvent(window) || is_changed;
        return is_changed;
    }
    bool Form3D::handle(GLFWwindow* window) {
        bool is_changed = false;
        is_changed = Container2D::handle(window) || is_changed;
        is_changed = Container3D::handle(window) || is_changed;
        return is_changed;
    }
    void Form3D::glDraw() const {
        MyBase::ControlCenter::getInstance().Enable3DMode();
        Container3D::glDraw();
        MyBase::ControlCenter::getInstance().Disable3DMode();
        MyBase::Container2D::glDraw();
    };
    void Form3D::glDrawTransparent() const {
        MyBase::ControlCenter::getInstance().Enable3DMode();
        Container3D::glDrawTransparent();
        MyBase::ControlCenter::getInstance().Disable3DMode();
        MyBase::Container2D::glDrawTransparent();
    };
    
    void Form3D::setReturnValue(const int& returnValue) {
        __returnValue = returnValue;
    }
    void Form3D::close() {
        __isOpen = false;
    }
    glm::vec2 Form3D::getPosition()   const {
        return {-1,-1};
    }
    glm::vec2 Form3D::getSize() const {
        return {2, 2};
    }
    void Form3D::setBackgroundColor(const MyBase::Color& color) {
        __backgroundColor = color;
    }
}