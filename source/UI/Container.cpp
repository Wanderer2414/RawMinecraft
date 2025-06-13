#include "Container.h"
#include "Controller.h"
namespace MyBase{

    Container::Container(): _currentFocus(-1), _currentHover(-1), _previosFocus(-1) {
    }
    Container::~Container() {
    
    }
    bool Container::setHover(const glm::vec2& position) {
        bool is_changed = Controller::setHover(position);
        _currentHover = -1;
        if (isHovered()) {
            glm::vec2 pos = position - getPosition();
            if (_currentFocus != -1) {
                is_changed = children[_currentFocus].first->setHover(pos) || is_changed;
                if (children[_currentFocus].first->isHovered()) _currentHover = _currentFocus;
            }
            for (int i = 0; i<children.size(); i++) {
                if (_currentHover ==-1 && _currentFocus != i) {
                    is_changed = children[i].first->setHover(pos) || is_changed;
                    if (children[i].first->isHovered()) _currentHover = i;
                }
                else if (i!=_currentHover) is_changed = children[i].first->setHover(false) || is_changed;
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
        __isFocus = focus;
        if (!focus) {
            for (auto& i:children) i.first->setFocus(false);
        }
        else _currentFocus = -1;
    }
    void Container::reset() {
        Controller::reset();
        _previosFocus = _currentFocus;
        for (auto& i:children) i.first->reset();
    }
    bool Container::handle(GLFWwindow* window) {
        bool is_changed = Controller::handle(window) || is_changed;
        for (auto& i:children) 
            is_changed = i.first->handle(window) || is_changed;
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
        if (_currentFocus <= children.size()) _currentFocus--;
        if (_currentHover <= children.size()) _currentHover--;
    }
    void Container::clear() {
        children.clear();
    }
    void Container::setPosition(const float& x, const float& y) {
        Controller::setPosition(x, y);
    }
    void Container::glDraw() const {
        for (auto& child:children) child.first->glDraw();
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