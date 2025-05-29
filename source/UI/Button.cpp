#include "Button.h"
#include "Controller.h"
#include "Global.h"
#include "SettingPackage.h"

template<typename T>
MyBase::Button<T>::Button(ButtonSetting& b_setting, const TextSetting& text_setting): _buttonSetting(&b_setting) {
    setTextPackage(text_setting);
    setButtonPackage(b_setting);
}

template<class T>
bool MyBase::Button<T>::contains(const sf::Vector2f& position) const {
    return T::contains(position);
}

template<class T>
MyBase::Button<T>::~Button() {
}

template <typename T>
_catch_function(MyBase::Button<T>, AfterCatch) {
    sf::Color color = _buttonSetting->normal_color;
    if (isPressed()) color = _buttonSetting->click_color;
    else if (isHovered()) color = _buttonSetting->hover_color;
    if (T::getFillColor() != color) {
        T::setFillColor(color);
        return true;
    }
    return false;
}
template <typename T>
sf::Vector2f MyBase::Button<T>::getPosition() const {
    return T::getPosition();
}

template <class T>
sf::FloatRect MyBase::Button<T>::getLocalBounds() const {
    return T::getLocalBounds();
}

template <class T>
sf::FloatRect MyBase::Button<T>::getGlobalBounds() const {
    return T::getGlobalBounds();
}
template<class T>
void MyBase::Button<T>::draw(sf::RenderTarget& target, sf::RenderStates state) const {
    T::draw(target, state);
    Text::draw(target, state);
}
template <class T>
bool MyBase::Button<T>::setHover(const bool& hover) {
    if (Controller::setHover(hover)) {
        if (isHovered()) T::setFillColor(_buttonSetting->hover_color);
        else T::setFillColor(_buttonSetting->normal_color);
        return true;
    }
    return false;
}
template <class T>
void MyBase::Button<T>::setTextPackage(const TextSetting& setting) {
    setCharacterSize(setting.font_size);
    setFillColor(setting.font_color);
    setFont(setting.font);
    setLetterSpacing(setting.spacing);
}
template <class T>
void MyBase::Button<T>::setButtonPackage(ButtonSetting& setting) {
    _buttonSetting = &setting;
    T::setFillColor(setting.normal_color);
}

template<class T>
void MyBase::Button<T>::update() {
    T::update();
    sf::Vector2f pos = T::getSize()/2.f;
    pos.y -= getCharacterSize()/2;
    pos.x -= Text::getLocalBounds().width/2;
    if (pos!=Text::getPosition()) Text::setPosition(pos);
}
template <class T>
void MyBase::Button<T>::setPosition(const float& x, const float& y) {
    T::setPosition(x, y);
}

template <class T>
void MyBase::Button<T>::setPosition(const sf::Vector2f& position) {
    MyBase::Button<T>::setPosition(position.x, position.y);
}

template <class T>
void MyBase::Button<T>::setString(const std::string& value) {
    Text::move({-Text::getLocalBounds().width/2, 0});
    Text::setString(value);
    Text::move({Text::getLocalBounds().width/2, 0});
}
template <class T>
sf::Vector2f MyBase::Button<T>::getSize() const {
    return T::getSize();
}