#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "Alarm.h"
#include "Button.h"
#include "Global.h"
#include "Rectangle.h"
#include "RoundedRectangle.h"
#include "SettingPackage.h"

template<typename T>
class Textbox: public Button<T> {
public:
    Textbox(const ButtonSetting& button_setting = ButtonSetting::Default, const TextSetting& text_setting = TextSetting::Default);

    virtual bool        isEnter() const,
                        isTextChanged() const;

    virtual void        setString(const string& text) override,
                        setHideString(const string& text),
                        reset() override,
                        setSize(const float& x, const float& y) override,
                        setPosition(const float& x, const float& y) override;
    string              getString() const;
    virtual void        update() override;
    virtual Vector2f    getPosition() const override;

protected:
    virtual             catch_function(CatchEvent) override;
    virtual             catch_function(AfterCatch) override;
    virtual             handle_function(handle) override;
    virtual void draw(RenderTarget& target, RenderStates state) const override;
    string              m_content,
                        hide_string;
    bool                m_is_enter,
                        m_is_text_changed,
                        m_is_multiline,
                        m_show_cusor;
    size_t              m_cursor_index;
    RectangleShape      cursor;
    Alarm               alarm;
private:
};
template class Textbox<MC::Rectangle>;
template class Textbox<RoundedRectangle>;
#endif
