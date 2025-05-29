#include "Textbox.h"
#include "Controller.h"
#include "Global.h"
#include "SettingPackage.h"
namespace MyBase {

    template<typename T>
    Textbox<T>::Textbox(const ButtonSetting& setting, const TextSetting& text_setting) {
        Button<T>::setTextPackage(text_setting);
        T::setFillColor(setting.normal_color);
        _cursorIndex = 0;
        _isEnter = _showCCursor = _isTextChanged = false;
        _cursor.setSize({2, text_setting.font_size});
        _isMultiLine = false;
        _alarm.setDuration(0.5);
    }
    
    template<typename T>
    bool Textbox<T>::isEnter() const {
        return _isEnter;
    }
    template<typename T>
    bool Textbox<T>::isTextChanged() const {
        return _isTextChanged;
    }
    template <typename T> 
    void Textbox<T>::reset() {
        Controller::reset();
        _isTextChanged = _isEnter  = false;
    }
    template<typename T>
    _catch_function(Textbox<T>, CatchEvent) {
        using KCode = sf::Keyboard::Scancode;
        #define isPress(X) sf::Keyboard::isKeyPressed(sf::Keyboard::X)
    
        bool is_changed = Button<T>::CatchEvent(window, event, state);
        if (Button<T>::isPressed()) {
            _cursorIndex = getString().size();
            _cursor.setPosition(Button<T>::findCharacterPos(_cursorIndex));
            is_changed = true;
        }
        else if (Button<T>::isFocus()) {
            if (event.type == sf::Event::KeyPressed) {
                char insert = 0;
                if (KCode::A <= event.key.scancode && event.key.scancode <= KCode::Z) {
                    if (isPress(LShift) || isPress(RShift))
                        insert = event.key.scancode + 'A';
                    else insert = event.key.scancode + 'a';
                } else if (KCode::Num1 <= event.key.scancode && event.key.scancode <= KCode::Num0) {
                    if (isPress(LShift) || isPress(RShift)) {
                        switch (event.key.scancode) {
                            case KCode::Num1: insert = '!'; break;
                            case KCode::Num2: insert = '@'; break;
                            case KCode::Num3: insert = '#'; break;
                            case KCode::Num4: insert = '$'; break;
                            case KCode::Num5: insert = '%'; break;
                            case KCode::Num6: insert = '^'; break;
                            case KCode::Num7: insert = '&'; break;
                            case KCode::Num8: insert = '*'; break;
                            case KCode::Num9: insert = '('; break;
                            default: insert = ')';
                        }
                    } else {
                        if (event.key.scancode != KCode::Num0) 
                            insert = event.key.scancode -KCode::Num1 + '1';
                        else insert = '0';
                    }
                } else if (KCode::Numpad1 <= event.key.scancode && event.key.scancode<=KCode::Numpad0) {
                    if (event.key.scancode != KCode::Numpad0) {
                        insert = event.key.scancode - KCode::Numpad1 + '1';
                    } else insert = '0';
                }
                else switch (event.key.scancode) {
                    case KCode::Enter: {
                        if (!_isMultiLine) {
                            _isEnter = true;
                            Button<T>::setFocus(false);
                            is_changed = true;
                        }
                        else insert = '\n';
                    }
                        break;
                    case KCode::Escape: {
    
                    }
                        break;
                    case KCode::Backspace: {
                        if (_cursorIndex) {
                            _content.erase(_content.begin()+_cursorIndex - 1);
                            _cursorIndex--;
                            is_changed = true;
                        } else return false;
                    }
                        break;
                    case KCode::Tab: break;
                    case KCode::Space: insert = ' '; break;
                    case KCode::Hyphen: {
                        if (isPress(LShift) || isPress(RShift))
                            insert = '_'; else insert = '-';
                    }
                        break;
                    case KCode::Equal: {
                        if (isPress(LShift) || isPress(RShift))
                            insert = '+'; else insert = '=';
                    }
                        break;
                    case KCode::LBracket: {
                        if (isPress(LShift) || isPress(RShift))
                            insert = '{'; else insert = '[';
                    }
                        break;
                    case KCode::RBracket: {
                        if (isPress(LShift) || isPress(RShift))
                            insert = '}'; else insert = ']';
                    }
                        break;
                    case KCode::Backslash: {
                        if (isPress(LShift) || isPress(RShift))
                            insert = '|'; else insert = '\\';
                    }
                        break;
                    case KCode::Semicolon: {
                        if (isPress(LShift) || isPress(RShift))
                            insert = ':'; else insert = ';';
                    }
                        break;
                    case KCode::Apostrophe: {
                        if (isPress(LShift) || isPress(RShift))
                            insert = '"'; else insert = '\'';
                    }
                        break;
                    case KCode::Grave: {
                        if (isPress(LShift) || isPress(RShift))
                            insert = '~'; else insert = '`';
                    }
                        break;
                    case KCode::Comma: {
                        if (isPress(LShift) || isPress(RShift))
                            insert = '<'; else insert = '.';
                    }
                        break;
                    case KCode::Period: {
                        if (isPress(LShift) || isPress(RShift))
                            insert = '>'; else insert = '.';
                    }
                        break;
                    case KCode::Slash: {
                        if (isPress(LShift) || isPress(RShift))
                            insert = '?'; else insert = '/';
                    }
                        break;
                    case KCode::Delete: {
                        if (_cursorIndex < _content.size()) {
                            _content.erase(_content.begin() + _cursorIndex);
                            is_changed = true;
                        }
                    }
                        break;
                    case KCode::NumpadDivide: {
                        insert = '/';
                    }
                        break;
                    case KCode::NumpadMultiply: {
                        insert = '*';
                    }
                        break;
                    case KCode::NumpadMinus: {
                        insert = '-';
                    }
                        break;
                    case KCode::NumpadPlus: {
                        insert = '+';
                    }
                        break;
                    case KCode::NumpadEqual: {
                        insert = '=';
                    }
                        break;
                    case KCode::NumpadEnter: {
                        insert = '\n';
                    }
                        break;
                    case KCode::NumpadDecimal: {
                        insert = '.';
                    }
                        break;
                    case KCode::Left: {
                        if (_cursorIndex) {
                            _cursorIndex--;
                            is_changed = true;
                        }
                    }
                        break;
                    case KCode::Right: {
                        if (_cursorIndex<_content.size()) {
                            _cursorIndex++;
                            is_changed = true;
                        }
                    }
                        break;
                    default: break;
                }
                if (insert) {
                    _content.insert(_content.begin() + _cursorIndex, insert);
                    _cursorIndex++;
                }
            }
        }
        return is_changed;
    }
    template <typename T>
    _catch_function(Textbox<T>, AfterCatch) {
        bool is_changed = Button<T>::AfterCatch(window, event, state);
        if (sf::Text::getString() != _content && Button<T>::isFocus()) {
            if (_content.size()) _isTextChanged = true;
            setString(_content);
            is_changed = true;
        }
        if (_content.empty() && sf::Text::getString() != _hideString && !Button<T>::isFocus()) {
            setString("");
            is_changed = true;
        }
        _showCCursor = true;
        if (is_changed) update();
        _cursor.setPosition(Button<T>::findCharacterPos(_cursorIndex));
        return is_changed;
    }
    template<typename T>
    _handle_function(Textbox<T>, handle) {
        bool is_changed = false;
        if (Button<T>::isFocus() && _alarm.get()) {
            _showCCursor = !_showCCursor;
            is_changed = true;
        }
        return is_changed;
    }
    
    template<typename T>
    std::string Textbox<T>::getString() const {
        return _content;
    }
    
    template<typename T>
    sf::Vector2f Textbox<T>::getPosition() const {
        return Button<T>::getPosition();
    }
    template<typename T>
    void Textbox<T>::setSize(const float& x, const float& y) {
        Button<T>::setSize(x, y);
    }
    template <typename T> 
    void Textbox<T>::setPosition(const float& x, const float& y) {
        Button<T>::setPosition(x, y);
    }
    template<typename T>
    void Textbox<T>::update() {
        Button<T>::update();
    }
    template <typename T> 
    void Textbox<T>::setHideString(const std::string& value) {
        Button<T>::setString(value);
        _hideString = value;
        update();
    }
    template <typename T>
    void Textbox<T>::setString(const std::string& value) {
        _content = value;
        if (value=="" && !Button<T>::isFocus()) Button<T>::setString(_hideString);
        else Button<T>::setString(value);
        if (_cursorIndex > value.size()) _cursorIndex = value.size();
        if (_cursorIndex < 0) _cursorIndex = 0;
        update();
    }
    template <typename T>
    void Textbox<T>::draw(sf::RenderTarget& target, sf::RenderStates state) const {
        T::draw(target, state);
        
        sf::RenderTexture texture;
        texture.create(T::getSize().x, T::getSize().y);
        texture.clear(sf::Color::Transparent);
        if (_cursor.getPosition().x > T::getSize().x) 
            texture.setView((sf::View)sf::FloatRect(_cursor.getPosition().x - T::getSize().x + 5, 0, T::getSize().x, T::getSize().y));
        else texture.setView((sf::View)sf::FloatRect(0, 0, T::getSize().x, T::getSize().y));
    
        sf::Text::draw(texture, state);
        if (Button<T>::isFocus() &&_showCCursor) texture.draw(_cursor, state);
        texture.display();
        sf::Sprite sprite(texture.getTexture());
        sprite.setPosition(getPosition());
        target.draw(sprite, state);
    }
}