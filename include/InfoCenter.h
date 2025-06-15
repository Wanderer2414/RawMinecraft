#ifndef INFO_CENTER_H
#define INFO_CENTER_H
#include "Global.h"
namespace MyCraft {
    class InfoCenter {
        public:
            InfoCenter(const float& width, const float& height);
            ~InfoCenter();
            
            static InfoCenter* Default;
            float getWindowRatio() const;
            glm::vec2 getWindowSize() const;
            glm::vec2 getWindowHalf() const;
        private:
            glm::vec2       __windowSize, __windowHalfSize;
    };
};
#endif