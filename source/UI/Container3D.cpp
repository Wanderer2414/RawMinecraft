#include "Container3D.h"
#include "Controller3D.h"
#include "Global.h"

Container3D::Container3D() {
    hovered_controller = focus_control = -1;
}
Container3D::~Container3D() {

}
bool Container3D::setHover(const Ray3f& sight) {
    bool is_changed = Controller3D::setHover(sight);
    hovered_controller = -1;
    if (isHovered()) {
        if (focus_control != -1) {
            is_changed = children[focus_control].first->setHover(sight) || is_changed;
            if (children[focus_control].first->isHovered()) hovered_controller = focus_control;
        }
        for (int i = 0; i<children.size(); i++) {
            if (hovered_controller==-1 && focus_control != i) {
                is_changed = children[i].first->setHover(sight) || is_changed;
                if (children[i].first->isHovered()) hovered_controller = i;
            }
            else if (i!=hovered_controller) is_changed = children[i].first->setHover(false) || is_changed;
        }
    }
    else setHover(false);
    return is_changed;
}
bool Container3D::setHover(const bool& hover) {
    if (Controller3D::setHover(hover) && !hover) {
        for (int i = 0; i<children.size(); i++) children[i].first->setHover(false);
        return true;
    }
    return false;
}
void Container3D::setFocus(const bool& focus) {
    pIsFocus = focus;
    if (!focus) {
        for (auto& i:children) i.first->setFocus(false);
    }
    else focus_control = -1;
}
void Container3D::reset() {
    Controller3D::reset();
    old_focus = focus_control;
    for (auto& i:children) i.first->reset();
}
_catch_function(Container3D, CatchEvent) {
    bool is_changed = Controller3D::CatchEvent(window, event, state) || is_changed;
    if (focus_control != -1) {
        is_changed = children[focus_control].first->CatchEvent(window, event) || is_changed;
    }
    if (hovered_controller != -1 && hovered_controller != focus_control) {
        is_changed = children[hovered_controller].first->CatchEvent(window, event) || is_changed;
    }
    return is_changed;
}
_catch_function(Container3D, AfterCatch) {
    bool is_changed = Controller3D::AfterCatch(window, event, state) || is_changed;
    if (focus_control != -1) {
        is_changed = children[focus_control].first->AfterCatch(window, event) || is_changed;
        if (!children[focus_control].first->isFocus()) focus_control = -1;
    }
    if (hovered_controller != -1 && hovered_controller != focus_control) {
        is_changed = children[hovered_controller].first->AfterCatch(window, event) || is_changed;
        if (children[hovered_controller].first->isFocus()) focus_control = hovered_controller;
    }
    if (old_focus != focus_control) is_changed = true;
    return is_changed;
}
_handle_function(Container3D, handle) {
    bool is_changed = false;
    for (int i = children.size()-1; i>=0; i--) {
        is_changed = children[i].first->handle(window, state) || is_changed;
    }
    return is_changed;
}
size_t Container3D::size() const {
    return children.size();
}
Controller3D* Container3D::operator[](const size_t& index) {
    return children[index].first;
}
void Container3D::insert(Controller3D* controller, const int& layer) {
    int i = 0;
    while (i<children.size() && children[i].second<=layer) i++;
    children.insert(children.begin()+i, {controller, layer}); 
}
void Container3D::erase(Controller3D* controller) {
    int i = 0;
    while (i<children.size() && children[i].first != controller) i++;
    if (i<children.size()) children.erase(children.begin() + i);
    if (focus_control <= children.size()) focus_control--;
    if (hovered_controller <= children.size()) hovered_controller--;
}
void Container3D::clear() {
    children.clear();
}

void Container3D::draw(RenderTarget& target, RenderStates state) const {
    for (const auto& [child, layer]:children) 
        if (focus_control == -1 || child != children[focus_control].first) target.draw(*child, state);
    if (focus_control != -1) target.draw(*children[focus_control].first, state);
};
void Container3D::glDraw() const {
    for (const auto& [child, layer]:children) 
        child->glDraw();
    
}
void Container3D::update() {
    for (auto& [child, layer]:children) child->update();
}