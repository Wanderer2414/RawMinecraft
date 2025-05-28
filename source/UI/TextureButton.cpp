#include "TextureButton.h"
#include "Controller.h"
#include "Global.h"
#include "SFML/Graphics/Sprite.hpp"
namespace MyBase {

    Button<Texture>::Button() {
        shared_texture_index = 0;
    }
    Button<Texture>::~Button() {
    
    }
    bool Button<Texture>::setHover(const bool& hover) {
        bool ans = Controller::setHover(hover);
        if (ans) {
            auto& [normal, hover, click] = shared_storage[shared_texture_index];
            if (isHovered()) Sprite::setTexture(*hover);
            else Sprite::setTexture(*normal);
        }
        return ans;
    }
    bool Button<Texture>::contains(const Vector2f& position) const {
        FloatRect rect = getGlobalBounds();
        return rect.contains(position);
    }
    uint Button<Texture>::getTextureIndex() const {
        return shared_texture_index;
    }
    void Button<Texture>::reset() {
        Controller::reset();
    }
    _catch_function(Button<Texture>,AfterCatch) {
        bool is_changed = Controller::AfterCatch(window, event, state);
        auto& [normal, hover, click] = shared_storage[shared_texture_index];
        Texture* texture = normal;
        if (isPressed()) texture = click;
        else if (isHovered()) texture = hover;
        if (getTexture() != texture) {
            is_changed = true;
            Sprite::setTexture(*texture);
        }
        return is_changed;
    }
    
    
    Vector2f Button<Texture>::getPosition() const {
        return Sprite::getPosition();
    }
    
    void Button<Texture>::setPosition(const float& x, const float& y) {
        Sprite::setPosition(x, y);
    }
    void Button<Texture>::setPosition(const Vector2f& position) {
        setPosition(position.x, position.y);
    }
    
    void Button<Texture>::setSize(const float& width, const float& height) {
        auto sz = getTextureRect().getSize();
        setScale(width/sz.x, height/sz.y);
    }
    
    void Button<Texture>::setTexture(const int& index, Texture* normal, Texture* hover, Texture* click) {
        if (index>=shared_storage.size()) shared_storage.resize(index+1);
        shared_storage[index] = {normal, hover, click};
        if (index == shared_texture_index) Sprite::setTexture(*normal);
    }
    void Button<Texture>::next_texture() {
        setIndex((shared_texture_index+1)%shared_storage.size());
    }
    void Button<Texture>::back_texture() {
        setIndex((shared_texture_index-1)%shared_storage.size());
    }
    void Button<Texture>::setIndex(const int& index) {
        if (shared_texture_index != index) {
            shared_texture_index = index;
            auto [normal, hover, click] = shared_storage[shared_texture_index];
            Sprite::setTexture(*normal);
        }
    }
    void Button<Texture>::draw(RenderTarget& target, RenderStates state) const {
        Sprite::draw(target, state);
    }
}