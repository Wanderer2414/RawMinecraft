#include "Form3D.h"
#include "Color.h"
#include "Container2D.h"
#include "Container3D.h"
#include "ControlCenter.h"
#include "Global.h"
#include "Shape.h"
#include "ShapeManager.h"

namespace MyBase3D {
    Form3D::Form3D(const int& index): __formIndex(index), __returnValue(INT_MIN), __backgroundColor(WHITE), __frameCount(0) {
        __sensitiveClock.setDuration(10);
        MyBase::ShapeManager::Default->createShape(__pauseScreen, {2,2});
        setFillColor({0,0,0, 120});
        ShapeContainer::setPosition({-1,-1});
        __sensitiveClock.setDuration(30);
    }
    Form3D::~Form3D() {
        MyBase::ShapeManager::Default->removeShape(__pauseScreen, {2,2});
    }
    bool Form3D::contains(const glm::vec2& position) const {
        return true;
    }
    float Form3D::getMaxFps() const {
        return 1.0f*CLOCKS_PER_SEC*__frameCount/clock();
    }
    int Form3D::run(GLFWwindow* window) {
        bool is_changed = true;
        glDisable(GL_DEPTH_TEST);
        while (!glfwWindowShouldClose(window)) {
            Container2D::reset();
            glfwPollEvents();
            is_changed = setHover(MyBase::ControlCenter::Default->getCursorPos(window)) || is_changed;
            is_changed = catchEvent(window) || is_changed;
            if (__sensitiveClock.get()) {
                __sensitiveClock.restart();
                is_changed = sensitiveHandle(window) || is_changed;
            }
            is_changed = handle(window) || is_changed;
            if (is_changed) {
                glm::vec4 color = __backgroundColor.getColor();
                glClearColor(color.r, color.g, color.b, color.a);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glDraw();
                glfwSwapBuffers(window);
            }
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }
            if (__returnValue!=INT_MIN) return __returnValue;
            is_changed = 0;
            __frameCount++;
        }
        return __formIndex;
    }
    bool Form3D::sensitiveHandle(GLFWwindow* window) {
        bool is_changed = Container2D::sensitiveHandle(window);
        is_changed = Container3D::sensitiveHandle(window) || is_changed;
        return is_changed;
    }
    bool Form3D::catchEvent(GLFWwindow* window) {
        bool is_changed = Container2D::catchEvent(window);
        is_changed = Container3D::catchEvent(window) || is_changed;
        return is_changed;
    }
    bool Form3D::handle(GLFWwindow* window) {
        bool is_changed = Container2D::handle(window);
        is_changed = Container3D::handle(window) || is_changed;
        return is_changed;
    }
    void Form3D::glDraw() const {
        MyBase::ControlCenter::Default->disable3DMode();
        MyBase::Container2D::glDraw();
        MyBase::ControlCenter::Default->enable3DMode();
        Container3D::glDraw();
    };
    glm::vec2 Form3D::getPosition()   const {
        return {-1,-1};
    }
    glm::vec2 Form3D::getSize() const {
        return {2, 2};
    }
    void Form3D::pauseScreen(GLFWwindow* window) {
        draw(__pauseScreen);

        glfwSwapBuffers(window);
        glm::vec4 color = __backgroundColor.getColor();
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDraw();
        draw(__pauseScreen);
    }
    void Form3D::setBackgroundColor(const MyBase::Color& color) {
        __backgroundColor = color;
    }
    void Form3D::setSensitiveTime(const size_t& time) {
        __sensitiveClock.setDuration(time);
    }
}