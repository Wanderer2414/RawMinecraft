#include "Alarm.h"

Alarm::Alarm() {
    m_duration = 0;
}
Alarm::~Alarm() {

}
bool Alarm::get() {
    bool ans = clock.getElapsedTime().asSeconds()>=m_duration;
    if (ans) clock.restart();
    return ans;
}
void Alarm::setDuration(const float& duration) {
    m_duration = duration;
}