#ifndef VERTICAL_BOX_H
#define VERTICAL_BOX_H
#include "Container.h"
#include "Global.h"
#include "Rectangle.h"
namespace MyBase {

class VerticalBox: public Container, public MyBase::Rectangle {
    public:
        VerticalBox();
        ~VerticalBox();
        virtual bool    contains(const sf::Vector2f& position) const override,
                        setHover(const sf::Vector2f& position) override;
        virtual void    insert(Controller* controller, const int& layer = 0) override,
                        erase(Controller* controller)                       override;
        virtual sf::Vector2f getPosition() const override,
                        getSize() const override;
        virtual void    setPosition(const float& x, const float& y) override,
                        update() override;
    protected:
        virtual catch_function(CatchEvent) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
        float       _delta;
    private:
        float __height;
    };
}
#endif