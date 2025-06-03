#ifndef CLOCK_H
#define CLOCK_H

#include "Global.h"
namespace MyBase {

    class Clock {
        public:
            Clock();
            ~Clock();
            bool        get();
            float       getDuration() const;
            void        setDuration(const float& duration);
        private:
            std::size_t       __previous          = 0;
            std::size_t       __duration          = 0;
        };
        
}
#endif