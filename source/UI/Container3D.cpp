#include "Container3D.h"
namespace MyBase3D {

    Container3D::Container3D() {
    }
    Container3D::~Container3D() {
    
    }
    bool Container3D::catchEvent(GLFWwindow* window) {
        bool is_changed = Controller3D::catchEvent(window);
        for (auto& i:__children) is_changed = i->catchEvent(window) || is_changed;
        return is_changed;
    }
    bool Container3D::handle(GLFWwindow* window) {
        bool is_changed = Controller::handle(window);
        for (auto& i: __children) is_changed = i->handle(window) || is_changed;
        return is_changed;
    }
    std::size_t Container3D::size() const {
        return __children.size();
    }
    Controller3D* Container3D::operator[](const size_t& index) {
        if (index<__children.size()) return __children[index];
        return 0;
    }
    void Container3D::insert(Controller3D* controller) {
        __children.push_back(controller);
    }
    void Container3D::erase(Controller3D* controller) {
        int i = 0;
        while (i<__children.size() && __children[i] != controller) i++;
        __children.erase(__children.begin() + i);
    }

    void Container3D::clear() {
        __children.clear();
    }
    void Container3D::glDraw() const {
        for (auto& child:__children) 
            if (child->isVisible()) child->glDraw();
    }
    void Container3D::update() {
        for (auto& child:__children) child->update();
    }
}