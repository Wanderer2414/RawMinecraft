#ifndef ALARM_H
#define ALARM_H
#include "Controller.h"

class Alarm: public Controller {
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