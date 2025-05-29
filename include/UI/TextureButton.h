#ifndef TEXTURE_BUTTON_H
#define TEXTURE_BUTTON_H
#include "Button.h"
#include "Controller.h"
namespace MyBase {

template <>
class Button<sf::Texture>: public Controller, public sf::Sprite {
public:
    Button();
    ~Button();
    uint            getTextureIndex() const;
    virtual bool    setHover(const bool& hover) override;
    virtual void    setTexture(const int& index, sf::Texture* normal, sf::Texture* hover, sf::Texture* click),
                    reset() override,
                    setPosition(const float& x, const float& y) override,
                    setPosition(const sf::Vector2f& position),
                    setSize(const float& width, const float& height),
                    setIndex(const int& index),
                    next_texture(),
                    back_texture();
    virtual sf::Vector2f getPosition() const override;
protected:
    uint            _textureIndex;
    virtual         catch_function(AfterCatch) override;
    virtual void    draw(sf::RenderTarget& target, sf::RenderStates state) const override;
    virtual bool    contains(const sf::Vector2f& position) const override;
    std::vector<std::tuple<sf::Texture*, sf::Texture*, sf::Texture*>> _storages;
private:
};
}
#endif