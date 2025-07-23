#include "Form.h"
#include "ControlCenter.h"
#include "Global.h"
#include "Shape.h"
#include "ShapeManager.h"

namespace MyBase {
    Form::Form(const int& index): __formIndex(index), __returnValue(INT_MIN), __backgroundColor(WHITE) {
        __sensitiveClock.setDuration(10);
        ShapeManager::getInstance().createShape(__pauseScreen, {2,2});
        setFillColor({0,0,0, 120});
        ShapeContainer::setPosition({-1,-1});
    }
    Form::~Form() {
        ShapeManager::getInstance().removeShape(__pauseScreen, {2,2});
    }
    bool Form::contains(const glm::vec2& position) const {
        return true;
    }
    int Form::run(GLFWwindow* window) {
        bool is_changed = true;
        glDisable(GL_DEPTH_TEST);
        while (!glfwWindowShouldClose(window)) {
            ControlCenter::getInstance().Reset();
            Container2D::reset();
            glfwPollEvents();
            is_changed = Container2D::setHover(ControlCenter::getInstance().getCursorPos(window)) || is_changed;
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
            if (__returnValue!=INT_MIN) return __returnValue;
            is_changed = 0;
        }
        return __formIndex;
    }

    int Form::getReturnForm() const {
        return __returnValue;
    }
    int Form::getFormIndex() {
        return __formIndex;
    }
    void Form::setReturnForm(const int& returnValue) {
        __returnValue = returnValue;
    }

    glm::vec2 Form::getPosition()   const {
        return {-1,-1};
    }
    glm::vec2 Form::getSize() const {
        return {2, 2};
    }
    void Form::pauseScreen(GLFWwindow* window) {
        ControlCenter::getInstance().BindSubScreen();
        glDraw();
        draw(__pauseScreen);
        ControlCenter::getInstance().UnbindSubScreen();
    }
    void Form::setBackgroundColor(const Color& color) {
        __backgroundColor = color;
    }
    void Form::setSensitiveTime(const size_t& time) {
        __sensitiveClock.setDuration(time);
    }
}