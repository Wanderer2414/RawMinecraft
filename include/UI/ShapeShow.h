#ifndef SHAPESHOE_H
#define SHAPESHOE_H
#include "Circle.h"
#include "Controller.h"
#include "Rectangle.h"
#include "RoundedRectangle.h"
#include "SettingPackage.h"
template <typename T>
class ShapeShow: public Controller, public T {
public:
    ShapeShow(ButtonSetting* b_setting = &ButtonSetting::Default);
    ~ShapeShow();

    virtual Vector2f getPosition() const override;
    virtual void     setPosition(const float& x, const float& y) override,
                    setPosition(const Vector2f& position);
protected:
    virtual void draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
private:
};
template class ShapeShow<RoundedRectangle>;
template class ShapeShow<Rectangle>;
template class ShapeShow<Circle>;
#endif