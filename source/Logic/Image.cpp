#include "Image.h"
#include "Color.h"
#include "Global.h"
#include <stdexcept>
namespace MyBase {
    Image::Image(const unsigned int& width, const unsigned int& height, const Color& color): __size(width,height), __position(0, 0) {
        __pixel = new Color[width*height];
        for (int i = 0; i<width*height; i++) __pixel[i] = color;
    }
    Image::~Image() {
        delete[] __pixel;
    }
    const glm::ivec2& Image::getSize() const {
        return __size;
    }
    const glm::ivec2& Image::getPosition() const {
        return __position;
    }
    void Image::setPosition(const glm::ivec2& position) {
        __position = position;
    }
    void Image::setColor(const int& x, const int& y, const MyBase::Color& color) {
        glm::ivec2 offset = glm::ivec2(x,y)-__position;
        if (offset.x<0 || offset.x>=__size.x || offset.y<0 || offset.y>=__size.y)
            throw std::runtime_error("Out range of image!");
        __pixel[offset.y*__size.x + offset.x] = color;
    }
    const Color& Image::getColor(const int& x, const int& y) const {
        glm::ivec2 offset = glm::ivec2(x,y)-__position;
        if (offset.x<0 || offset.x>=__size.x || offset.y<0 || offset.y>=__size.y)
            throw std::runtime_error("Out range of image!");
        return __pixel[offset.y*__size.x + offset.x];
    }
    void Image::save(const std::string& source) {
        stbi_write_png(source.c_str(), __size.x, __size.y, 4, __pixel, __size.x*sizeof(Color));
    }
}