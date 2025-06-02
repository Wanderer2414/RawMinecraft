#include "InfoCenter.h" 
namespace MyCraft {
    InfoCenter::InfoCenter(const float& width, const float& height): __windowSize(width, height), __windowHalfSize(__windowSize/2.f) {
        
    }
    InfoCenter::~InfoCenter() {
        glfwTerminate();
    }

    float InfoCenter::getWindowRatio() const {
        return __windowSize.x/__windowSize.y;
    }
    glm::vec2 InfoCenter::getWindowSize() const {
        return __windowSize;
    }
    glm::vec2 InfoCenter::getWindowHalf() const {
        return __windowHalfSize;    
    }
}