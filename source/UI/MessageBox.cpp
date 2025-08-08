#include "MessageBox.h"
#include "ControlCenter.h"
#include "Controller2D.h"
#include "Global.h"
#include "Shape.h"
#include "ShapeManager.h"
namespace MyBase {
    MessageBox::MessageBox(): __isOpen(false), __size(2,2), __roundness(0), __returnValue(-1) {
        ShapeContainer::setPosition({-1,-1});
        ShapeContainer::setFillColor({0,0,0,120});
        ShapeManager::getInstance().createShape(__background, __size, __roundness);
    }
    MessageBox::~MessageBox() {
        ShapeManager::getInstance().removeShape(__background, __size, __roundness);
    }

    int MessageBox::getReturnValue() const {
        return __returnValue;
    }

    glm::vec2 MessageBox::getPosition() const {
        return ShapeContainer::getPosition();
    };
    glm::vec2 MessageBox::getSize() const {
        return __size;
    };
    bool MessageBox::catchEvent(GLFWwindow* window) {
        bool is_changed = Container2D::catchEvent(window);
        if (__exitCondition() && ControlCenter::getInstance().IsMouseClicked() && !isHovered()) close();
        return is_changed;
    }
    void MessageBox::setSize(const glm::vec2& size, const float& roundness) {
        if (size!=__size || __roundness != roundness) {
            ShapeManager::getInstance().removeShape(__background, __size, __roundness);
            __size = size; __roundness = roundness;
            ShapeManager::getInstance().createShape(__background, __size, __roundness);
        }
    }
    void MessageBox::__open(GLFWwindow*) {}
    void MessageBox::__close(GLFWwindow*) {}
    int MessageBox::open(GLFWwindow* window) {
        __isOpen = true;
        bool is_changed = true;
        __open(window);
        setHover(0);
        while (!glfwWindowShouldClose(window) && __isOpen) {
            ControlCenter::getInstance().Reset();
            Container2D::reset();
            glfwPollEvents();
            is_changed = Container2D::setHover(ControlCenter::getInstance().getCursorPos(window)) || is_changed;
            is_changed = catchEvent(window) || is_changed;
            is_changed = handle(window) || is_changed;
            if (is_changed) {
                ControlCenter::getInstance().Disable3DMode();
                glClearColor(0,0,0, 1);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                ControlCenter::getInstance().DrawSavedScreen();
                glDraw();
                glDrawTransparent();
                glfwSwapBuffers(window);
            }
            if (__exitCondition() && ControlCenter::getInstance().IsKeyPressed() && glfwGetKey(window, GLFW_KEY_ESCAPE)) {
                close();
            }
            is_changed = 0;
        }
        ControlCenter::getInstance().Reset();
        __close(window);
        return __returnValue;
    }
    bool MessageBox::__exitCondition() {
        return true;
    }
    void MessageBox::setReturnValue(const int& value) {
        __returnValue = value;
    }
    void MessageBox::close() {
        __isOpen = false;
    }
    bool MessageBox::contains(const glm::vec2& position) const {
        return __background.contains(position-getPosition());
    }
    void MessageBox::glDraw() const {
        ShapeContainer::draw(__background);
        Container2D::glDraw();
    }
    void MessageBox::glDrawTransparent() const {
        Container2D::glDrawTransparent();
    }
}