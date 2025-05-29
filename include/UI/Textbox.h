#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "Alarm.h"
#include "Button.h"
#include "Global.h"
#include "Rectangle.h"
#include "RoundedRectangle.h"
#include "SettingPackage.h"
namespace MyBase {

    template<typename T>
    class Textbox: public Button<T> {
    public:
        Textbox(const ButtonSetting& button_setting = ButtonSetting::Default, const TextSetting& text_setting = TextSetting::Default);
    
        virtual bool            isEnter() const,
                                isTextChanged() const;
    
        virtual void            setString(const std::string& text)          override,
                                setHideString(const std::string& text),
                                reset()                                     override,
                                setSize(const float& x, const float& y)     override,
                                setPosition(const float& x, const float& y) override;
        std::string             getString()                         const;
        virtual void            update()                                    override;
        virtual sf::Vector2f    getPosition()                       const   override;
    
    protected:
        virtual             catch_function(CatchEvent)  override;
        virtual             catch_function(AfterCatch)  override;
        virtual             handle_function(handle)     override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const override;
        std::string         _content,
                            _hideString;
        bool                _isEnter,
                            _isTextChanged,
                            _isMultiLine,
                            _showCCursor;
        std::size_t         _cursorIndex;
        sf::RectangleShape  _cursor;
        Alarm               _alarm;
    private:
    };
    template class Textbox<Rectangle>;
    template class Textbox<RoundedRectangle>;
}
#endif
