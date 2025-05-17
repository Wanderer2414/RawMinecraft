#ifndef ALARM_H
#define ALARM_H
#include "Global.h"

class Alarm {
public:
    Alarm();
    ~Alarm();
    virtual bool    get();
    virtual void    setDuration(const float& duration);
private:
    float           m_duration;
    Clock           clock;
};
#endif