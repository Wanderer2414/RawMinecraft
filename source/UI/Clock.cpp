#include "Clock.h"
#include "General.h"
#include <cmath>
namespace MyBase {

    Clock::Clock() {

    }
    bool Clock::get() const {
        if (!__duration) return true;
        std::size_t current = GetTime();
        if (current-__previous>=__duration) {
            return true;
        }
        return false;
    }
    float Clock::getDuration() const {
        return __duration;
    }
    void Clock::setDuration(const float& duration) {
        __duration = duration;
        if (__duration) {
            __previous = GetTime();
        }
    }
    void Clock::stop() {
        __previous = INFINITY;
    }
    void Clock::restart() {
        __previous = GetTime();
    }
    Clock::~Clock() {
    
    }
}