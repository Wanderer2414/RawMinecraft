#include "Container.h"
#include "Controller.h"
#include "Global.h"
namespace MyBase{

    Container::Container(): _currentFocus(-1), _currentHover(-1), _previosFocus(-1) {
    }
    Container::~Container() {
    
    }
    bool Container::setHover(const sf::Vector2f& position) {
        bool is_changed = Controller::setHover(position);
        _currentHover = -1;
        if (isHovered()) {
            sf::Vector2f pos = position - getPosition();
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
    _catch_function(Container, CatchEvent) {
        bool is_changed = Controller::CatchEvent(window, event, state) || is_changed;
        if (_currentFocus != -1) {
            is_changed = children[_currentFocus].first->CatchEvent(window, event) || is_changed;
        }
        if (_currentHover != -1 && _currentHover != _currentFocus) {
            is_changed = children[_currentHover].first->CatchEvent(window, event) || is_changed;
        }
        return is_changed;
    }
    _catch_function(Container, AfterCatch) {
        bool is_changed = Controller::AfterCatch(window, event, state) || is_changed;
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
    _handle_function(Container, handle) {
        bool is_changed = false;
        for (int i = children.size()-1; i>=0; i--) {
            is_changed = children[i].first->handle(window, state) || is_changed;
        }
        return is_changed;
    }
    std::size_t Container::size() const {
        return children.size();
    }
    sf::Vector2f Container::getPosition() const {
        return {0, 0};
    }
    sf::Vector2f Container::getSize() const {
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
        if (_currentFocus <= children.size()) _currentFocus--;
        if (_currentHover <= children.size()) _currentHover--;
    }
    void Container::clear() {
        children.clear();
    }
    void Container::setPosition(const float& x, const float& y) {
        Controller::setPosition(x, y);
    }
    
    void Container::draw(sf::RenderTarget& target, sf::RenderStates state) const {
        sf::RenderTexture texture;
        texture.create(getSize().x, getSize(). y);
        texture.clear(sf::Color::Transparent);
        texture.setView((sf::View)sf::FloatRect(0, 0, getSize().x, getSize().y));
        for (const auto& [child, layer]:children) 
            if (_currentFocus == -1 || child != children[_currentFocus].first) texture.draw(*child, state);
        if (_currentFocus != -1) texture.draw(*children[_currentFocus].first, state);
        texture.display();
        sf::Sprite sprite(texture.getTexture());
        sprite.setPosition(getPosition());
        target.draw(sprite);
    };
    void Container::update() {
        for (auto& [child, layer]:children) child->update();
    }
}