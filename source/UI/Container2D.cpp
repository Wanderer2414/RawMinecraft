#include "Container2D.h"
namespace MyBase{

    Container2D::Container2D(): __currentFocus(-1), __currentHover(-1), __previosFocus(-1) {
    }
    Container2D::~Container2D() {
    
    }
    bool Container2D::setHover(const glm::vec2& position) {
        bool is_changed = Controller2D::setHover(position);
        int old_hover = __currentHover;
        if (isHovered()) {
            for (auto& child:__permanentChildren) is_changed = child->setHover(position) || is_changed;
            if (__currentHover != -1) {
                is_changed =__children[__currentHover].first->setHover(position) || is_changed;
                if (!__children[__currentHover].first->isHovered()) {
                    __currentHover = -1;
                };
            }
            if (__currentHover == -1) {
                if (__currentFocus != -1) {
                    is_changed = __children[__currentFocus].first->setHover(position) || is_changed;
                    if (__children[__currentFocus].first->isHovered()) __currentHover = __currentFocus;
                }
                for (int i = 0; i<__children.size(); i++) {
                    if (__currentFocus != i) {
                        is_changed = __children[i].first->setHover(position) || is_changed;
                        if (__children[i].first->isHovered()) {
                            __currentHover = i;
                            break;
                        }
                    }
                }
            }
            is_changed = (old_hover != __currentHover) || is_changed;
        }
        else setHover(false);
        return is_changed;
    }
    const int& Container2D::getCurrentFocusIndex() const {
        return __currentFocus;
    }
    const int& Container2D::__getCurrentFocus() const {
        return __currentFocus;
    }
    const int& Container2D::__getCurrentHover() const {
        return __currentHover;
    }
    bool Container2D::__setCurrentHover(const unsigned int& index) {
        if (__currentFocus == index) return false;
        __currentHover = index;
        return true;
    }
    void Container2D::__setCurrentFocus(const unsigned int& index) {
        __currentFocus = index;
    }
    bool Container2D::__unsetCurrentHover() {
        if (__currentHover==-1) return false;
        __currentHover = -1;
        return true;
    }
    void Container2D::__unsetCurrentFocus() {
        __currentFocus = -1;
    }
    bool Container2D::setHover(const bool& hover) {
        if (Controller2D::setHover(hover)) {
            if (!hover) {
                for (auto& child:__permanentChildren) child->setHover(false);
                if (__currentHover!=-1) __children[__currentHover].first->setHover(false);
                return true;
            }
            else for (auto& child:__permanentChildren) child->setHover(true);
        }
        return false;
    }
    Controller2D* Container2D::getCurrentFocus() const {
        if (__currentFocus>=0) return __children[__currentFocus].first;
        else return 0;
    }
    void Container2D::setFocus(const bool& focus) {
        Controller2D::setFocus(focus);
        if (!focus) {
            for (auto& i:__children) i.first->setFocus(false);
        }
        else __currentFocus = -1;
    }
    void Container2D::reset() {
        Controller2D::reset();
        __previosFocus = __currentFocus;
        for (auto& i:__permanentChildren) i->reset();
        for (auto& i:__children) i.first->reset();
    }
    bool Container2D::sensitiveHandle(GLFWwindow* window) {
        bool is_changed = false;
        for (auto& child:__permanentChildren) is_changed = child->sensitiveHandle(window) || is_changed;
        for (int i = 0; i<__children.size(); i++) { 
            is_changed = __children[i].first->sensitiveHandle(window) || is_changed;
            if (__children[i].first->isFocus()) __currentFocus = i;
        }
        is_changed = Controller::sensitiveHandle(window) || is_changed;
        return is_changed;
    }
    bool Container2D::catchEvent(GLFWwindow* window) {
        bool is_changed = false;
        for (auto& i:__permanentChildren) is_changed = i->catchEvent(window) || is_changed;
        if (__currentFocus>=0) {
            is_changed = __children[__currentFocus].first->catchEvent(window) || is_changed;
            if (!__children[__currentFocus].first->isFocus()) __currentFocus = -1;
        }
        if (__currentHover>=0 && __currentHover!=__currentFocus) {
            is_changed = __children[__currentHover].first->catchEvent(window) || is_changed;
            if (__children[__currentHover].first->isFocus()) __currentFocus = __currentHover;
        }
        is_changed = Controller2D::catchEvent(window) || is_changed;
        return is_changed;
    }
    bool Container2D::handle(GLFWwindow* window) {
        bool is_changed = false;
        for (auto& i: __permanentChildren) is_changed = i->handle(window) || is_changed;
        for (int i = 0; i<__children.size(); i++)  {
            __children[i].first->handle(window) || is_changed;
            if (__children[i].first->isFocus()) __currentFocus = i;
        }
        is_changed = Controller::handle(window) || is_changed;
        return is_changed;
    }
    std::size_t Container2D::size() const {
        return __children.size() + __permanentChildren.size();
    }
    Controller2D* Container2D::operator[](const size_t& index) {
        if (index<__children.size()) return __children[index].first;
        else return __permanentChildren[index-__children.size()];
    }
    void Container2D::insert(Controller2D* controller, const int& layer) {
        // Replace by RTree
        // if (__childrenPosition.x + __childrenSize.x < controller->getPosition().x + controller->getSize().x) {
        //     __childrenSize.x = controller->getPosition().x + controller->getSize().x - controller->getPosition().x;
        // }
        // if (__childrenPosition.y + __childrenSize.y < controller->getPosition().y + controller->getSize().y) {
        //     __childrenSize.y = controller->getPosition().y + controller->getSize().y - controller->getPosition().y;
        // }
        // __childrenPosition.x = std::min(__childrenPosition.x, controller->getPosition().x);
        // __childrenPosition.y = std::min(__childrenPosition.y, controller->getPosition().y);
        int i = 0;
        while (i<__children.size() && __children[i].second<=layer) i++;
        __children.insert(__children.begin()+i, {controller, layer}); 
    }
    void Container2D::erase(Controller2D* controller) {
        int i = 0;
        while (i<__children.size() && __children[i].first != controller) i++;
        if (i<__children.size()) __children.erase(__children.begin() + i);
        if (__currentFocus <= __children.size()) __currentFocus--;
        if (__currentHover <= __children.size()) __currentHover--;
    }

    void Container2D::insertPermanent(Controller2D* controller) {
        __permanentChildren.push_back(controller);
    }
    void Container2D::clear() {
        __children.clear();
    }
    void Container2D::glDraw() const {
        for (auto& child:__permanentChildren) 
            if (child->isVisible()) child->glDraw();
        for (int i = __children.size()-1; i>=0; i--) 
            if (i!=__currentFocus && __children[i].first->isVisible()) __children[i].first->glDraw();
        if (__currentFocus>=0 && __children[__currentFocus].first->isVisible()) __children[__currentFocus].first->glDraw();
    }
    void Container2D::update() {
        for (auto& [child, layer]:__children) child->update();
        for (auto& child:__permanentChildren) child->update();
    }
}