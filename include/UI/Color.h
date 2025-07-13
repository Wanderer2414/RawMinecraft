#ifndef COLOR_H
#define COLOR_H
#include "Global.h"
namespace MyBase {
    struct Color {
        unsigned char red, green, blue, alpha;
        glm::vec4 getColor() const;
        bool operator==(const Color& color) const;
        bool operator!=(const Color& color) const;
    };
    #define BLACK MyBase::Color({0,0,0,255})
    #define WHITE MyBase::Color({255,255,255,255})
    #define RED MyBase::Color({255,0,0,255})
    #define GREEN MyBase::Color({0,255,0,255})
    #define BLUE MyBase::Color({0,0,255,255})

}
#endif