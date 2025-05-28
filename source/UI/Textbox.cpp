#include "Textbox.h"
#include "Controller.h"
#include "Global.h"
#include "SettingPackage.h"
namespace MyBase {

    template<typename T>
    Textbox<T>::Textbox(const ButtonSetting& setting, const TextSetting& text_setting) {
        Button<T>::setTextPackage(text_setting);
        T::setFillColor(setting.normal_color);
        m_cursor_index = 0;
        m_is_enter = m_show_cusor = m_is_text_changed = false;
        cursor.setSize({2, text_setting.font_size});
        m_is_multiline = false;
        alarm.setDuration(0.5);
    }
    
    template<typename T>
    bool Textbox<T>::isEnter() const {
        return m_is_enter;
    }
    template<typename T>
    bool Textbox<T>::isTextChanged() const {
        return m_is_text_changed;
    }
    template <typename T> 
    void Textbox<T>::reset() {
        Controller::reset();
        m_is_text_changed = m_is_enter  = false;
    }
    template<typename T>
    _catch_function(Textbox<T>, CatchEvent) {
        using KCode = Keyboard::Scancode;
        #define isPress(X) Keyboard::isKeyPressed(Keyboard::X)
    
        bool is_changed = Button<T>::CatchEvent(window, event, state);
        if (Button<T>::isPressed()) {
            m_cursor_index = getString().size();
            cursor.setPosition(Button<T>::findCharacterPos(m_cursor_index));
            is_changed = true;
        }
        else if (Button<T>::isFocus()) {
            if (event.type == Event::KeyPressed) {
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
                        if (!m_is_multiline) {
                            m_is_enter = true;
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
                        if (m_cursor_index) {
                            m_content.erase(m_content.begin()+m_cursor_index - 1);
                            m_cursor_index--;
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
                        if (m_cursor_index < m_content.size()) {
                            m_content.erase(m_content.begin() + m_cursor_index);
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
                        if (m_cursor_index) {
                            m_cursor_index--;
                            is_changed = true;
                        }
                    }
                        break;
                    case KCode::Right: {
                        if (m_cursor_index<m_content.size()) {
                            m_cursor_index++;
                            is_changed = true;
                        }
                    }
                        break;
                    default: break;
                }
                if (insert) {
                    m_content.insert(m_content.begin() + m_cursor_index, insert);
                    m_cursor_index++;
                }
            }
        }
        return is_changed;
    }
    template <typename T>
    _catch_function(Textbox<T>, AfterCatch) {
        bool is_changed = Button<T>::AfterCatch(window, event, state);
        if (Text::getString() != m_content && Button<T>::isFocus()) {
            if (m_content.size()) m_is_text_changed = true;
            setString(m_content);
            is_changed = true;
        }
        if (m_content.empty() && Text::getString() != hide_string && !Button<T>::isFocus()) {
            setString("");
            is_changed = true;
        }
        m_show_cusor = true;
        if (is_changed) update();
        cursor.setPosition(Button<T>::findCharacterPos(m_cursor_index));
        return is_changed;
    }
    template<typename T>
    _handle_function(Textbox<T>, handle) {
        bool is_changed = false;
        if (Button<T>::isFocus() && alarm.get()) {
            m_show_cusor = !m_show_cusor;
            is_changed = true;
        }
        return is_changed;
    }
    
    template<typename T>
    string Textbox<T>::getString() const {
        return m_content;
    }
    
    template<typename T>
    Vector2f Textbox<T>::getPosition() const {
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
    void Textbox<T>::setHideString(const string& value) {
        Button<T>::setString(value);
        hide_string = value;
        update();
    }
    template <typename T>
    void Textbox<T>::setString(const string& value) {
        m_content = value;
        if (value=="" && !Button<T>::isFocus()) Button<T>::setString(hide_string);
        else Button<T>::setString(value);
        if (m_cursor_index > value.size()) m_cursor_index = value.size();
        if (m_cursor_index < 0) m_cursor_index = 0;
        update();
    }
    template <typename T>
    void Textbox<T>::draw(RenderTarget& target, RenderStates state) const {
        T::draw(target, state);
        
        RenderTexture texture;
        texture.create(T::getSize().x, T::getSize().y);
        texture.clear(Color::Transparent);
        if (cursor.getPosition().x > T::getSize().x) 
            texture.setView((View)FloatRect(cursor.getPosition().x - T::getSize().x + 5, 0, T::getSize().x, T::getSize().y));
        else texture.setView((View)FloatRect(0, 0, T::getSize().x, T::getSize().y));
    
        Text::draw(texture, state);
        if (Button<T>::isFocus() && m_show_cusor) texture.draw(cursor, state);
        texture.display();
        Sprite sprite(texture.getTexture());
        sprite.setPosition(getPosition());
        target.draw(sprite, state);
    }
}