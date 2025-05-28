#include "VerticalBox.h"
#include "Container.h"
#include "Rectangle.h"
namespace MyBase {

    VerticalBox::VerticalBox() {
        delta = 0;
        height = 0;
    }
    VerticalBox::~VerticalBox() {
    
    }
    bool VerticalBox::contains(const Vector2f& position) const {
        return Rectangle::contains(position);
    }
    Vector2f VerticalBox::getPosition() const {
        return Rectangle::getPosition();
    }
    Vector2f VerticalBox::getSize() const {
        return Rectangle::getSize();
    }
    bool VerticalBox::setHover(const Vector2f& position) {
        bool is_changed = Controller::setHover(position);
        hovered_controller = -1;
        if (isHovered()) {
            Vector2f pos = position - getPosition();
            pos.y -= delta;
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
        height = 0;
        for (auto& [controller, layer]: children)
            if (float tmp = controller->getPosition().y + controller->getSize().y; tmp>height) height = tmp;
        if (height < getSize().y) delta = 0;
        else delta = clamp(delta, getSize().y -height, 0.f);
    }
    void VerticalBox::setPosition(const float& x, const float& y) {
        Container::setPosition(x, y);
        Rectangle::setPosition(x, y);
    }
    _catch_function(VerticalBox, CatchEvent) {
        bool is_changed = Container::CatchEvent(window, event, state);
        if (isHovered() && event.type == Event::MouseWheelScrolled) {
            delta -= event.mouseWheelScroll.delta*10;
            if (height < getSize().y) delta = 0;
            delta = clamp(delta, getSize().y -height, 0.f);
            is_changed = true;
        }
        return is_changed;
    }
    void VerticalBox::draw(RenderTarget& target, RenderStates state) const {
        RenderTexture texture;
        texture.create(getSize().x, getSize(). y);
        texture.clear(Color::Transparent);
        texture.setView((View)FloatRect(0, -delta, getSize().x, getSize().y));
        for (const auto& [child, layer]:children) 
            if (focus_control == -1 || child != children[focus_control].first) texture.draw(*child, state);
        if (focus_control != -1) texture.draw(*children[focus_control].first, state);
        texture.display();
        Sprite sprite(texture.getTexture());
        sprite.setPosition(getPosition());
        target.draw(sprite);
    }
}