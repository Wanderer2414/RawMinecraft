#include "Color.h"

namespace MyBase {
    
    glm::vec4 Color::getColor() const {
        return glm::vec4(red, green, blue, alpha)/256.f;
    }
    bool Color::operator==(const Color& color) const {
        return (red == color.red) && (blue == color.blue) && (green == color.green) && (alpha == color.alpha);
    }
    bool Color::operator!=(const Color& color) const {
        return (red != color.red) || (blue != color.blue) || (green != color.green) || (alpha != color.alpha);
    }

}