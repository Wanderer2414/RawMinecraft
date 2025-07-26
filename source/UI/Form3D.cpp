#include "Form3D.h"
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
        __sensitiveClock.setDuration(10);
        MyBase::ShapeManager::getInstance().createShape(__pauseScreen, {2,2});
        setFillColor({0,0,0, 120});
        ShapeContainer::setPosition({-1,-1});
        __sensitiveClock.setDuration(30);
    }
    Form3D::~Form3D() {
        MyBase::ShapeManager::getInstance().removeShape(__pauseScreen, {2,2});
    }
    bool Form3D::contains(const glm::vec2& position) const {
        return true;
    }
    float Form3D::getAverageFps() const {
        return 1.0f*(CLOCKS_PER_SEC)*__frameCount/(clock()-__startClock);
    }
    float Form3D::getCurrentFps() const {
        return 1.0f*CLOCKS_PER_SEC/__deltaClock;
    }
    int Form3D::run(GLFWwindow* window) {
        __currentClock = __startClock = clock();
        bool is_changed = true;
        __isOpen = true;
        while (!glfwWindowShouldClose(window) && __isOpen) {
            MyBase::ControlCenter::getInstance().Reset();
            Container2D::reset();
            glfwPollEvents();
            is_changed = setHover(MyBase::ControlCenter::getInstance().getCursorPos(window)) || is_changed;
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
            is_changed = 0;
            __deltaClock = clock() - __currentClock;
            __currentClock = clock();
            __frameCount++;
        }
        return __returnValue;
    }
    bool Form3D::sensitiveHandle(GLFWwindow* window) {
        bool is_changed = camera.sensitiveHandle(window);
        is_changed = Container2D::sensitiveHandle(window) || is_changed;
        is_changed = Container3D::sensitiveHandle(window) || is_changed;
        return is_changed;
    }
    bool Form3D::catchEvent(GLFWwindow* window) {
        bool is_changed = camera.catchEvent(window);
        is_changed = Container2D::catchEvent(window) || is_changed;
        is_changed = Container3D::catchEvent(window) || is_changed;
        return is_changed;
    }
    bool Form3D::handle(GLFWwindow* window) {
        bool is_changed = camera.handle(window);
        is_changed = Container2D::handle(window) || is_changed;
        is_changed = Container3D::handle(window) || is_changed;
        return is_changed;
    }
    void Form3D::glDraw() const {
        MyBase::ControlCenter::getInstance().Enable3DMode();
        Container3D::glDraw();
        camera.glDraw();
        MyBase::ControlCenter::getInstance().Disable3DMode();
        MyBase::Container2D::glDraw();
    };
    void Form3D::update() {
        camera.update();
    }
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
    void Form3D::pauseScreen(GLFWwindow* window) {
        MyBase::ControlCenter::getInstance().BindSubScreen();
        glDraw();
        draw(__pauseScreen);
        MyBase::ControlCenter::getInstance().UnbindSubScreen();
    }
    void Form3D::setBackgroundColor(const MyBase::Color& color) {
        __backgroundColor = color;
    }
    void Form3D::setSensitiveTime(const size_t& time) {
        __sensitiveClock.setDuration(time);
    }
}