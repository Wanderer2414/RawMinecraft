#include "Container.h"
#include "Controller.h"
#include "Global.h"

Container::Container() {
    hovered_controller = focus_control = -1;
}
Container::~Container() {

}
bool Container::setHover(const Vector2f& position) {
    bool is_changed = Controller::setHover(position);
    hovered_controller = -1;
    if (m_is_hovered) {
        Vector2f pos = position - getPosition();
        if (focus_control != -1) {
            is_changed = children[focus_control].first->setHover(pos) || is_changed;
            if (children[focus_control].first->isHovered()) hovered_controller = focus_control;
        }
        for (int i = 0; i<children.size(); i++) {
            if (hovered_controller==-1 && focus_control != i) {
                is_changed = children[i].first->setHover(pos) || is_changed;
                if (children[i].first->isHovered()) hovered_controller = i;
            }
            else if (i!=hovered_controller) is_changed = children[i].first->setHover(false) || is_changed;
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
void Container::setFocus(const bool& focus) {
    m_is_focus = focus;
    if (!focus) {
        for (auto& i:children) i.first->setFocus(false);
    }
    else focus_control = -1;
}
void Container::reset() {
    Controller::reset();
    old_focus = focus_control;
    for (auto& i:children) i.first->reset();
}
_catch_function(Container, CatchEvent) {
    bool is_changed = Controller::CatchEvent(window, event, state) || is_changed;
    if (focus_control != -1) {
        is_changed = children[focus_control].first->CatchEvent(window, event) || is_changed;
    }
    if (hovered_controller != -1 && hovered_controller != focus_control) {
        is_changed = children[hovered_controller].first->CatchEvent(window, event) || is_changed;
    }
    return is_changed;
}
_catch_function(Container, AfterCatch) {
    bool is_changed = Controller::AfterCatch(window, event, state) || is_changed;
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
_handle_function(Container, handle) {
    bool is_changed = false;
    for (int i = children.size()-1; i>=0; i--) {
        is_changed = children[i].first->handle(window, state) || is_changed;
    }
    return is_changed;
}
size_t Container::size() const {
    return children.size();
}
Vector2f Container::getPosition() const {
    return {0, 0};
}
Vector2f Container::getSize() const {
    return {0, 0};
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
    if (focus_control <= children.size()) focus_control--;
    if (hovered_controller <= children.size()) hovered_controller--;
}
void Container::clear() {
    children.clear();
}
void Container::setPosition(const float& x, const float& y) {
    Controller::setPosition(x, y);
}

void Container::draw(RenderTarget& target, RenderStates state) const {
    RenderTexture texture;
    texture.create(getSize().x, getSize(). y);
    texture.clear(Color::Transparent);
    texture.setView((View)FloatRect(0, 0, getSize().x, getSize().y));
    for (const auto& [child, layer]:children) 
        if (focus_control == -1 || child != children[focus_control].first) texture.draw(*child, state);
    if (focus_control != -1) texture.draw(*children[focus_control].first, state);
    texture.display();
    Sprite sprite(texture.getTexture());
    sprite.setPosition(getPosition());
    target.draw(sprite);
};
void Container::update() {
    for (auto& [child, layer]:children) child->update();
}