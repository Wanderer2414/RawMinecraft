#ifndef BUTTON_H
#define BUTTON_H
#include "Circle.h"
#include "Controller.h"
#include "Global.h"
#include "Rectangle.h"
#include "RoundedRectangle.h"
#include "SettingPackage.h"
namespace MyBase {
    template<typename T>
    class Button: public Controller, public Text, public T {
    public:
        Button(ButtonSetting& button_setting = ButtonSetting::Default, const TextSetting& text_setting = TextSetting::Default);
        ~Button();

        virtual bool    setHover(const bool& hover) override;
        virtual void    setTextPackage(const TextSetting& setting),
                        setButtonPackage(ButtonSetting& setting),
                        setPosition(const float& x, const float& y) override,
                        setPosition(const Vector2f& position),
                        setString(const string& value),
                        update() override;
        FloatRect       getLocalBounds() const,
                        getGlobalBounds() const;
        Vector2f        getPosition() const override,
                        getSize() const override;
    protected:
        virtual         catch_function(AfterCatch) override;
        virtual void    draw(RenderTarget& target, RenderStates state) const override;
        virtual bool    contains(const Vector2f& position) const override;
        ButtonSetting   *button_setting;
    private:
    };
    template class Button<RoundedRectangle>;
    template class Button<Rectangle>;
    template class Button<Circle>;
};
#endif