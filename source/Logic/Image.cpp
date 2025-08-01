#include "Image.h"
#include "Color.h"
#include "Global.h"
namespace MyBase {
    Image::Image(const unsigned int& width, const unsigned int& height, const Color& color): __size(width,height) {
        __pixel = new Color[width*height];
        for (int i = 0; i<width*height; i++) __pixel[i] = color;
    }
    Image::~Image() {
        delete[] __pixel;
    }
    const glm::ivec2& Image::getSize() const {
        return __size;
    }
    void Image::setColor(const unsigned int& x, const unsigned int& y, const MyBase::Color& color) {
        __pixel[y*__size.x + x] = color;
    }
    const Color& Image::getColor(const unsigned int& x, const unsigned int& y) const {
        return __pixel[y*__size.x + x];
    }
    void Image::save(const std::string& source) {
        stbi_write_png(source.c_str(), __size.x, __size.y, 4, __pixel, __size.x*sizeof(Color));
    }
}