#include "Container.h"
#include "Controller.h"
namespace MyBase{

    Container::Container(): __currentFocus(-1), __currentHover(-1), __previosFocus(-1) {
    }
    Container::~Container() {
    
    }
    bool Container::setHover(const glm::vec2& position) {
        bool is_changed = Controller::setHover(position);
        __currentHover = -1;
        if (isHovered()) {
            glm::vec2 pos = position - getPosition();
            if (__currentFocus != -1) {
                is_changed = children[__currentFocus].first->setHover(pos) || is_changed;
                if (children[__currentFocus].first->isHovered()) __currentHover = __currentFocus;
            }
            for (int i = 0; i<children.size(); i++) {
                if (__currentHover ==-1 && __currentFocus != i) {
                    is_changed = children[i].first->setHover(pos) || is_changed;
                    if (children[i].first->isHovered()) __currentHover = i;
                }
                else if (i!=__currentHover) is_changed = children[i].first->setHover(false) || is_changed;
            }
        }
        else setHover(false);
        return is_changed;
    }
    bool Container::setHover(const bool& hover) {
        if (Controller::setHover(hover) && !hover) {
            for (int i = 0; i<children.size(); i++) children[i].first->setHover(false);
            return true;
        }
        return false;
    }
    Controller* Container::getCurrentFocus() const {
        if (__currentFocus>=0) return children[__currentFocus].first;
        else return 0;
    }
    void Container::setFocus(const bool& focus) {
        __isFocus = focus;
        if (!focus) {
            for (auto& i:children) i.first->setFocus(false);
        }
        else __currentFocus = -1;
    }
    void Container::reset() {
        Controller::reset();
        __previosFocus = __currentFocus;
        for (auto& i:children) i.first->reset();
    }
    bool Container::sensitiveHandle(GLFWwindow* window) {
        bool is_changed = Controller::sensitiveHandle(window);
        for (auto& child: children) 
            is_changed = child.first->sensitiveHandle(window) || is_changed;
        return is_changed;
    }
    bool Container::catchEvent(GLFWwindow* window) {
        bool is_changed = Controller::catchEvent(window);
        if (__currentFocus>=0) 
            is_changed = children[__currentFocus].first->catchEvent(window) || is_changed;
        return is_changed;
    }
    bool Container::handle(GLFWwindow* window) {
        bool is_changed = Controller::handle(window);
        for (int i = 0; i<children.size(); i++) 
            children[i].first->handle(window) || is_changed;
        return is_changed;
    }
    std::size_t Container::size() const {
        return children.size();
    }
    Controller* Container::operator[](const size_t& index) {
        return children[index].first;
    }
    void Container::insert(Controller* controller, const int& layer) {
        int i = 0;
        while (i<children.size() && children[i].second<=layer) i++;
        children.insert(children.begin()+i, {controller, layer}); 
    }
    void Container::erase(Controller* controller) {
        int i = 0;
        while (i<children.size() && children[i].first != controller) i++;
        if (i<children.size()) children.erase(children.begin() + i);
        if (__currentFocus <= children.size()) __currentFocus--;
        if (__currentHover <= children.size()) __currentHover--;
    }
    void Container::clear() {
        children.clear();
    }
    void Container::setPosition(const float& x, const float& y) {
        Controller::setPosition(x, y);
    }
    void Container::glDraw() const {
        for (int i = 0; i<children.size(); i++) 
            if (i!=__currentFocus) children[i].first->glDraw();
        if (__currentFocus>=0) children[__currentFocus].first->glDraw();
    }
    void Container::update() {
        for (auto& [child, layer]:children) child->update();
    }
    glm::vec2 Container::getPosition() const {
        return Controller::getPosition();
    }
    glm::vec2 Container::getSize() const {
        return Controller::getSize();
    }
}