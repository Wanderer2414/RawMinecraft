#include "Button.h"
#include "BaseShape.h"
#include "Controller.h"
#include "Global.h"
#include "SettingPackage.h"

template<typename T>
Button<T>::Button(ButtonSetting& b_setting, const TextSetting& text_setting): button_setting(&b_setting) {
    setTextPackage(text_setting);
    setButtonPackage(b_setting);
}

template<class T>
bool Button<T>::contains(const Vector2f& position) const {
    return T::contains(position);
}

template<class T>
Button<T>::~Button() {
}

template <typename T>
_catch_function(Button<T>, AfterCatch) {
    Color color = button_setting->normal_color;
    if (isPressed()) color = button_setting->click_color;
    else if (isHovered()) color = button_setting->hover_color;
    if (T::getFillColor() != color) {
        T::setFillColor(color);
        return true;
    }
    return false;
}
template <typename T>
Vector2f Button<T>::getPosition() const {
    return T::getPosition();
}

template <class T>
FloatRect Button<T>::getLocalBounds() const {
    return T::getLocalBounds();
}

template <class T>
FloatRect Button<T>::getGlobalBounds() const {
    return T::getGlobalBounds();
}
template<class T>
void Button<T>::draw(RenderTarget& target, RenderStates state) const {
    Shape::draw(target, state);
    Text::draw(target, state);
}
template <class T>
bool Button<T>::setHover(const bool& hover) {
    if (Controller::setHover(hover)) {
        if (isHovered()) T::setFillColor(button_setting->hover_color);
        else T::setFillColor(button_setting->normal_color);
        return true;
    }
    return false;
}
template <class T>
void Button<T>::setTextPackage(const TextSetting& setting) {
    setCharacterSize(setting.font_size);
    setFillColor(setting.font_color);
    setFont(setting.font);
    setLetterSpacing(setting.spacing);
}
template <class T>
void Button<T>::setButtonPackage(ButtonSetting& setting) {
    button_setting = &setting;
    T::setFillColor(setting.normal_color);
}

template<class T>
void Button<T>::update() {
    BaseShape::update();
    Vector2f pos = T::getSize()/2.f;
    pos.y -= getCharacterSize()/2;
    pos.x -= Text::getLocalBounds().width/2;
    if (pos!=Text::getPosition()) Text::setPosition(pos);
}
template <class T>
void Button<T>::setPosition(const float& x, const float& y) {
    T::setPosition(x, y);
}

template <class T>
void Button<T>::setPosition(const Vector2f& position) {
    Button<T>::setPosition(position.x, position.y);
}

template <class T>
void Button<T>::setString(const string& value) {
    Text::move({-Text::getLocalBounds().width/2, 0});
    Text::setString(value);
    Text::move({Text::getLocalBounds().width/2, 0});
}
template <class T>
Vector2f Button<T>::getSize() const {
    return T::getSize();
}