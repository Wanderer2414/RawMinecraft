#ifndef ALARM_H
#define ALARM_H
#include "Global.h"
namespace MyBase {
    class Alarm {
        public:
            Alarm();
            ~Alarm();
            virtual bool    get();
            virtual void    setDuration(const float& duration);
        private:
            float           __duration;
            sf::Clock       __clock;
    };
};
#endif