#include "ShapeShow.h"
#include "SettingPackage.h"
namespace MyBase {

    template <typename T>
    ShapeShow<T>::ShapeShow(ButtonSetting* b_setting) {
        T::setFillColor(b_setting->normal_color);
    }
    template <typename T>
    ShapeShow<T>::~ShapeShow() {
    
    }
    template <typename T>
    Vector2f ShapeShow<T>::getPosition() const {
        return T::getPosition();
    }
    
    template <typename T>
    void ShapeShow<T>::setPosition(const float& x, const float& y) {
        T::setPosition(x, y);
    }
    
    template <typename T>
    void ShapeShow<T>::setPosition(const Vector2f& position) {
        setPosition(position.x, position.y);
    }
    
    template <typename T>
    void ShapeShow<T>::draw(RenderTarget& target, RenderStates state) const {
        T::draw(target, state);
    }
}