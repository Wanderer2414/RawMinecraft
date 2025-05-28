#ifndef TEXTURE_BUTTON_H
#define TEXTURE_BUTTON_H
#include "Button.h"
#include "Controller.h"
namespace MyBase {

template <>
class Button<Texture>: public Controller, public Sprite {
public:
    Button();
    ~Button();
    uint            getTextureIndex() const;
    virtual bool    setHover(const bool& hover) override;
    virtual void    setTexture(const int& index, Texture* normal, Texture* hover, Texture* click),
                    reset() override,
                    setPosition(const float& x, const float& y) override,
                    setPosition(const Vector2f& position),
                    setSize(const float& width, const float& height),
                    setIndex(const int& index),
                    next_texture(),
                    back_texture();
    virtual Vector2f getPosition() const override;
protected:
    uint            shared_texture_index;
    virtual         catch_function(AfterCatch) override;
    virtual void    draw(RenderTarget& target, RenderStates state) const override;
    virtual bool    contains(const Vector2f& position) const override;
    vector<tuple<Texture*, Texture*, Texture*>> shared_storage;
private:
};
}
#endif