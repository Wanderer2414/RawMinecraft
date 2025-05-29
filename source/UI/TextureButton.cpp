#include "TextureButton.h"
#include "Controller.h"
#include "Global.h"
#include "SFML/Graphics/Sprite.hpp"
namespace MyBase {

    Button<sf::Texture>::Button() {
        _textureIndex = 0;
    }
    Button<sf::Texture>::~Button() {
    
    }
    bool Button<sf::Texture>::setHover(const bool& hover) {
        bool ans = Controller::setHover(hover);
        if (ans) {
            auto& [normal, hover, click] = _storages[_textureIndex];
            if (isHovered()) Sprite::setTexture(*hover);
            else Sprite::setTexture(*normal);
        }
        return ans;
    }
    bool Button<sf::Texture>::contains(const sf::Vector2f& position) const {
        sf::FloatRect rect = getGlobalBounds();
        return rect.contains(position);
    }
    uint Button<sf::Texture>::getTextureIndex() const {
        return _textureIndex;
    }
    void Button<sf::Texture>::reset() {
        Controller::reset();
    }
    _catch_function(Button<sf::Texture>,AfterCatch) {
        bool is_changed = Controller::AfterCatch(window, event, state);
        auto& [normal, hover, click] = _storages[_textureIndex];
        sf::Texture* texture = normal;
        if (isPressed()) texture = click;
        else if (isHovered()) texture = hover;
        if (getTexture() != texture) {
            is_changed = true;
            Sprite::setTexture(*texture);
        }
        return is_changed;
    }
    
    
    sf::Vector2f Button<sf::Texture>::getPosition() const {
        return Sprite::getPosition();
    }
    
    void Button<sf::Texture>::setPosition(const float& x, const float& y) {
        Sprite::setPosition(x, y);
    }
    void Button<sf::Texture>::setPosition(const sf::Vector2f& position) {
        setPosition(position.x, position.y);
    }
    
    void Button<sf::Texture>::setSize(const float& width, const float& height) {
        auto sz = getTextureRect().getSize();
        setScale(width/sz.x, height/sz.y);
    }
    
    void Button<sf::Texture>::setTexture(const int& index, sf::Texture* normal, sf::Texture* hover, sf::Texture* click) {
        if (index>=_storages.size()) _storages.resize(index+1);
        _storages[index] = {normal, hover, click};
        if (index == _textureIndex) Sprite::setTexture(*normal);
    }
    void Button<sf::Texture>::next_texture() {
        setIndex((_textureIndex+1)%_storages.size());
    }
    void Button<sf::Texture>::back_texture() {
        setIndex((_textureIndex-1)%_storages.size());
    }
    void Button<sf::Texture>::setIndex(const int& index) {
        if (_textureIndex != index) {
            _textureIndex = index;
            auto [normal, hover, click] = _storages[_textureIndex];
            Sprite::setTexture(*normal);
        }
    }
    void Button<sf::Texture>::draw(sf::RenderTarget& target, sf::RenderStates state) const {
        Sprite::draw(target, state);
    }
}