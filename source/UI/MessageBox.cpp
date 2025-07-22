#include "MessageBox.h"
#include "ControlCenter.h"
#include "Global.h"
#include "Shape.h"
#include "ShapeManager.h"
namespace MyBase {
    MessageBox::MessageBox(): __isOpen(true), __size(2,2), __roundness(0) {
        ShapeContainer::setPosition({-1,-1});
        ShapeContainer::setFillColor({0,0,0,120});
        __sensitiveClock.setDuration(10);
        ShapeManager::getInstance().createShape(__background, __size, __roundness);
    }
    MessageBox::~MessageBox() {
        ShapeManager::getInstance().removeShape(__background, __size, __roundness);
    }

    glm::vec2 MessageBox::getPosition() const {
        return ShapeContainer::getPosition();
    };
    glm::vec2 MessageBox::getSize() const {
        return __size;
    };
    bool MessageBox::catchEvent(GLFWwindow* window) {
        bool is_changed = Container2D::catchEvent(window);
        if (isPressed() && __getCurrentHover() == -1) close();
        return is_changed;
    }
    void MessageBox::setSize(const glm::vec2& size, const float& roundness) {
        if (size!=__size || __roundness != roundness) {
            ShapeManager::getInstance().removeShape(__background, __size, __roundness);
            __size = size;
            __roundness = roundness;
            ShapeManager::getInstance().createShape(__background, __size, __roundness);
        }
    }
    void MessageBox::open(GLFWwindow* window) {
        __isOpen = true;
        bool is_changed = true;
        glDisable(GL_DEPTH_TEST);
        while (!glfwWindowShouldClose(window) && __isOpen) {
            Container2D::reset();
            glfwPollEvents();
            is_changed = Container2D::setHover(ControlCenter::Default->getCursorPos(window)) || is_changed;
            is_changed = catchEvent(window) || is_changed;
            if (__sensitiveClock.get()) {
                __sensitiveClock.restart();
                is_changed = sensitiveHandle(window) || is_changed;
            }
            is_changed = handle(window) || is_changed;
            if (is_changed) {
                glClearColor(0,0,0, 1);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                ControlCenter::Default->DrawSavedScreen();
                glDraw();
                glfwSwapBuffers(window);
            }
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }
            is_changed = 0;
        }
    }
    void MessageBox::close() {
        __isOpen = false;
    }
       
    bool MessageBox::contains(const glm::vec2& position) const {
        return __isOpen;
    }
    void MessageBox::glDraw() const {
        ShapeContainer::draw(__background);
        Container2D::glDraw();
    }
}