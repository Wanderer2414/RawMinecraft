#include "Alarm.h"

MyBase::Alarm::Alarm(): __duration(0) {
}
MyBase::Alarm::~Alarm() {

}
bool MyBase::Alarm::get() {
    bool ans = __clock.getElapsedTime().asSeconds()>=__duration;
    if (ans) __clock.restart();
    return ans;
}
void MyBase::Alarm::setDuration(const float& duration) {
    __duration = duration;
}