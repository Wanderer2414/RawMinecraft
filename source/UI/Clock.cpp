#include "Clock.h"
#include "General.h"
namespace MyBase {

    Clock::Clock() {

    }
    bool Clock::get() {
        if (!__duration) return true;
        std::size_t current = GetTime();
        if (current-__previous>=__duration) {
            __previous = current;
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
    Clock::~Clock() {
    
    }
}