#include "Container3D.h"
#include "Controller3D.h"
#include "Global.h"
namespace MyBase3D {
    Container3D::Container3D() {
        _currentHover = _currentFocus = -1;
    }
    Container3D::~Container3D() {
    
    }
    bool Container3D::setHover(const Ray3f& sight) {
        bool is_changed = Controller3D::setHover(sight);
        _currentHover = -1;
        if (isHovered()) {
            if (_currentFocus != -1) {
                is_changed = children[_currentFocus].first->setHover(sight) || is_changed;
                if (children[_currentFocus].first->isHovered()) _currentHover = _currentFocus;
            }
            for (int i = 0; i<children.size(); i++) {
                if (_currentHover==-1 && _currentFocus != i) {
                    is_changed = children[i].first->setHover(sight) || is_changed;
                    if (children[i].first->isHovered()) _currentHover = i;
                }
                else if (i!=_currentHover) is_changed = children[i].first->setHover(false) || is_changed;
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
        __isFocus = focus;
        if (!focus) {
            for (auto& i:children) i.first->setFocus(false);
        }
        else _currentFocus = -1;
    }
    void Container3D::reset() {
        Controller3D::reset();
        _previosFocus = _currentFocus;
        for (auto& i:children) i.first->reset();
    }
    _catch_function(Container3D, CatchEvent) {
        bool is_changed = Controller3D::CatchEvent(window, event, state) || is_changed;
        if (_currentFocus != -1) {
            is_changed = children[_currentFocus].first->CatchEvent(window, event) || is_changed;
        }
        if (_currentHover != -1 && _currentHover != _currentFocus) {
            is_changed = children[_currentHover].first->CatchEvent(window, event) || is_changed;
        }
        return is_changed;
    }
    _catch_function(Container3D, AfterCatch) {
        bool is_changed = Controller3D::AfterCatch(window, event, state) || is_changed;
        if (_currentFocus != -1) {
            is_changed = children[_currentFocus].first->AfterCatch(window, event) || is_changed;
            if (!children[_currentFocus].first->isFocus()) _currentFocus = -1;
        }
        if (_currentHover != -1 && _currentHover != _currentFocus) {
            is_changed = children[_currentHover].first->AfterCatch(window, event) || is_changed;
            if (children[_currentHover].first->isFocus()) _currentFocus = _currentHover;
        }
        if (_previosFocus != _currentFocus) is_changed = true;
        return is_changed;
    }
    _handle_function(Container3D, handle) {
        bool is_changed = false;
        for (int i = children.size()-1; i>=0; i--) {
            is_changed = children[i].first->handle(window, state) || is_changed;
        }
        return is_changed;
    }
    std::size_t Container3D::size() const {
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
        if (_currentFocus <= children.size()) _currentFocus--;
        if (_currentHover <= children.size()) _currentHover--;
    }
    void Container3D::clear() {
        children.clear();
    }
    
    void Container3D::draw(sf::RenderTarget& target, sf::RenderStates state) const {
        for (const auto& [child, layer]:children) 
            if (_currentFocus == -1 || child != children[_currentFocus].first) target.draw(*child, state);
        if (_currentFocus != -1) target.draw(*children[_currentFocus].first, state);
    };
    void Container3D::glDraw() const {
        for (const auto& [child, layer]:children) 
            child->glDraw();
        
    }
    void Container3D::update() {
        for (auto& [child, layer]:children) child->update();
    }
}