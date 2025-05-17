#ifndef VERTICAL_BOX_H
#define VERTICAL_BOX_H
#include "Container.h"
#include "Global.h"
#include "Rectangle.h"
class VerticalBox: public Container, public Rectangle {
public:
    VerticalBox();
    ~VerticalBox();
    virtual bool    contains(const Vector2f& position) const override,
                    setHover(const Vector2f& position) override;
    virtual void    insert(Controller* controller, const int& layer = 0) override,
                    erase(Controller* controller)                       override;
    virtual Vector2f getPosition() const override,
                    getSize() const override;
    virtual void    setPosition(const float& x, const float& y) override,
                    update() override;
protected:
    virtual catch_function(CatchEvent) override;
    virtual void draw(RenderTarget& target, RenderStates states = RenderStates::Default) const override;
    float       delta;
private:
    float height;
};
#endif