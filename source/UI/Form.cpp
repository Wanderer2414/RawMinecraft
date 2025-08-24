#include "Form.h"
#include "ControlCenter.h"
#include "Controller2D.h"
#include "General.h"
#include "Global.h"
#include "Shape.h"
#include "ShapeManager.h"

namespace MyBase {
    Form::Form(const int& index): __formIndex(index), __returnValue(-1), __backgroundColor(WHITE), __isOpen(false), __backgroundMusic("assets/sounds/BackgroundSound.mp3") {}
    Form::~Form() {}
    bool Form::contains(const glm::vec2& position) const {
        return true;
    }
    int Form::run(GLFWwindow* window) {
        __isOpen = true;
        bool is_changed = true;
        while (!glfwWindowShouldClose(window) && __isOpen) {
            ControlCenter::getInstance().Reset();
            Container2D::reset();
            glfwPollEvents();
            is_changed = Container2D::setHover(ControlCenter::getInstance().getCursorPos(window)) || is_changed;
            is_changed = catchEvent(window) || is_changed;
            is_changed = handle(window) || is_changed;
            if (is_changed) {
                ControlCenter::getInstance().Disable3DMode();
                glm::vec4 color = __backgroundColor.getColor();
                glClearColor(color.r, color.g, color.b, color.a);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glDraw();
                glDrawTransparent();
                glfwSwapBuffers(window);
                __backgroundMusic.setVolume(Sound::volume);
            }
            __backgroundMusic.play();
            
            is_changed = 0;
        }
        return __returnValue;
    }

    int Form::getReturnValue() const {
        return __returnValue;
    }
    int Form::getFormIndex() {
        return __formIndex;
    }
    void Form::close() {
        __isOpen = false;
    }
    void Form::setReturnValue(const int& returnValue) {
        __returnValue = returnValue;
    }

    glm::vec2 Form::getPosition()   const {
        return {-1,-1};
    }
    glm::vec2 Form::getSize() const {
        return {2, 2};
    }
    void Form::setBackgroundColor(const Color& color) {
        __backgroundColor = color;
    }
    void Form::setBackgroundMusic(const std::string& filename) {
        __backgroundMusic = Sound(filename);
    }
}