#include "Controller3D.h"

namespace MyBase3D {
    Controller3D::Controller3D():__isVisible(true) {
    }
    Controller3D::~Controller3D() {
    
    }
    bool Controller3D::isVisible() const {
        return __isVisible;
    }
    void Controller3D::setVisible(const bool& visible) {
        __isVisible = visible;
    }
}