#include "Label.h"
#include "RoundedRectangle.h"

template<typename T>
Label<T>::Label(const TextSetting& text_setting) {
    setTextPackage(text_setting);
    T::setPosition(0, 0);
    T::setSize(100, 50);
    T::setFillColor(Color::Transparent);
    m_align = Left | Top;
}
template<typename T>
Label<T>::~Label() {

}

template<typename T>
Vector2f Label<T>::getPosition() const {
    return T::getPosition();
}

template<typename T>
void Label<T>::setTextPackage(const TextSetting& setting) {
    setCharacterSize(setting.font_size);
    Text::setFillColor(setting.font_color);
    setFont(setting.font);
    setLetterSpacing(setting.spacing);
    update();
}

template<typename T>
void Label<T>::setPosition(const float& x, const float& y) {
    T::setPosition(x, y);
    update();
}

template<typename T>
void Label<T>::setPosition(const Vector2f& position) {
    setPosition(position.x, position.y);
}

template<typename T>
void Label<T>::setAlign(const int& align) {
    m_align = align;
    update();
}

template<typename T>
void Label<T>::setSize(const float& width, const float& height) {
    T::setSize(width, height);
    update();
}

template<typename T>
void Label<T>::update() {
    Vector2f text_pos;
    if ((m_align & Left) == Left) {
        text_pos.x = getPosition().x;
    }
    else if ((m_align & Right) == Right) {
        text_pos.x = getPosition().x + T::getSize().x - Text::getGlobalBounds().width;
    }
    else {
        text_pos.x = T::getCenter().x - Text::getGlobalBounds().width/2;
    }
    if ((m_align & Top) == Top) {
        text_pos.y = getPosition().y;
    }
    else if ((m_align & Bottom) == Bottom) {
        text_pos.y = getPosition().y + T::getSize().y - Text::getGlobalBounds().height;
    }
    else {
        text_pos.y = T::getCenter().y - Text::getGlobalBounds().height/2;
    }
    if (text_pos != Text::getPosition()) {
        Text::setPosition(text_pos);
    }
}
template<typename T>
void Label<T>::setString(const string& value) {
    Text::setString(value);
    update();
}
template<typename T>
void Label<T>::draw(RenderTarget& target, RenderStates state) const {
    T::draw(target,state);
    Text::draw(target, state);
}