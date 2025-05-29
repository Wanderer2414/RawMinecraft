#include "VerticalBox.h"
#include "Container.h"
#include "Rectangle.h"
namespace MyBase {

    VerticalBox::VerticalBox(): _delta(0), __height(0) {
    }
    VerticalBox::~VerticalBox() {
    
    }
    bool VerticalBox::contains(const sf::Vector2f& position) const {
        return Rectangle::contains(position);
    }
    sf::Vector2f VerticalBox::getPosition() const {
        return Rectangle::getPosition();
    }
    sf::Vector2f VerticalBox::getSize() const {
        return Rectangle::getSize();
    }
    bool VerticalBox::setHover(const sf::Vector2f& position) {
        bool is_changed = Controller::setHover(position);
        _currentHover = -1;
        if (isHovered()) {
            sf::Vector2f pos = position - getPosition();
            pos.y -= _delta;
            if (_currentFocus != -1) {
                is_changed = children[_currentFocus].first->setHover(pos) || is_changed;
                if (children[_currentFocus].first->isHovered()) _currentHover = _currentFocus;
            }
            for (int i = 0; i<children.size(); i++) {
                if (_currentHover==-1 && _currentFocus != i) {
                    is_changed = children[i].first->setHover(pos) || is_changed;
                    if (children[i].first->isHovered()) _currentHover = i;
                }
                else if (i!=_currentHover) is_changed = children[i].first->setHover(false) || is_changed;
            }
        }
        else {
            for (int i = 0; i<children.size(); i++) is_changed = children[i].first->setHover(false) || is_changed;
        }
        return is_changed;
    }
    void VerticalBox::insert(Controller* controller, const int& layer) {
        Container::insert(controller, layer);
    }
    void VerticalBox::erase(Controller* controller) {
        Container::erase(controller);
    }
    void VerticalBox::update() {
        Container::update();
        Rectangle::update();
        __height = 0;
        for (auto& [controller, layer]: children)
            if (float tmp = controller->getPosition().y + controller->getSize().y; tmp>__height) __height = tmp;
        if (__height < getSize().y) _delta = 0;
        else _delta = std::clamp(_delta, getSize().y -__height, 0.f);
    }
    void VerticalBox::setPosition(const float& x, const float& y) {
        Container::setPosition(x, y);
        Rectangle::setPosition(x, y);
    }
    _catch_function(VerticalBox, CatchEvent) {
        bool is_changed = Container::CatchEvent(window, event, state);
        if (isHovered() && event.type == sf::Event::MouseWheelScrolled) {
            _delta -= event.mouseWheelScroll.delta*10;
            if (__height < getSize().y) _delta = 0;
            _delta = std::clamp(_delta, getSize().y -__height, 0.f);
            is_changed = true;
        }
        return is_changed;
    }
    void VerticalBox::draw(sf::RenderTarget& target, sf::RenderStates state) const {
        sf::RenderTexture texture;
        texture.create(getSize().x, getSize(). y);
        texture.clear(sf::Color::Transparent);
        texture.setView((sf::View)sf::FloatRect(0, -_delta, getSize().x, getSize().y));
        for (const auto& [child, layer]:children) 
            if (_currentFocus == -1 || child != children[_currentFocus].first) texture.draw(*child, state);
        if (_currentFocus != -1) texture.draw(*children[_currentFocus].first, state);
        texture.display();
        sf::Sprite sprite(texture.getTexture());
        sprite.setPosition(getPosition());
        target.draw(sprite);
    }
}