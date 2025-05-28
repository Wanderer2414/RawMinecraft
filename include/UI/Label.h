#ifndef LABEL_H
#define LABEL_H
#include "Controller.h"
#include "Rectangle.h"
#include "RoundedRectangle.h"
#include "SettingPackage.h"
namespace MyBase {

enum Align {
    Right = 1, Middle = 2, Left = 4,
    Top = 8, Center = 16, Bottom = 32
};

template <typename T>
class Label: public Controller, public Text, public T {
public:
    Label(const TextSetting& text_setting = TextSetting::Default);
    ~Label();

    virtual void setTextPackage(const TextSetting& package),
                setAlign(const int& align),
                setString(const string& value),
                setPosition(const float& x, const float& y) override,
                setPosition(const Vector2f& position),
                setSize(const float& width, const float& height) override,
                update() override;
    virtual Vector2f getPosition() const override;
protected:
    virtual void draw(RenderTarget& target, RenderStates state) const override;
    int         m_align;
private:
};
template class Label<Rectangle>;
template class Label<RoundedRectangle>;
}
#endif