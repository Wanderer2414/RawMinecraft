#include "Controller.h"

namespace MyBase {
    Controller::Controller() {};
    Controller::~Controller() {};
    bool Controller::catchEvent(GLFWwindow* window) {
        return false;
    }
    bool Controller::handle(GLFWwindow* window) {
        return false;
    }
    void Controller::reset() {}
    void Controller::update() {}
    void Controller::glDraw() const {}
    void Controller::glDrawTransparent() const {}
    
}