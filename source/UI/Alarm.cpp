#include "Alarm.h"

MyBase::Alarm::Alarm() {
    m_duration = 0;
}
MyBase::Alarm::~Alarm() {

}
bool MyBase::Alarm::get() {
    bool ans = clock.getElapsedTime().asSeconds()>=m_duration;
    if (ans) clock.restart();
    return ans;
}
void MyBase::Alarm::setDuration(const float& duration) {
    m_duration = duration;
}