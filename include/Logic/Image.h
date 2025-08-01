#ifndef IMAGE_H
#define IMAGE_H
#include "Color.h"
namespace MyBase {

    class Image {
    public:
        Image(const unsigned int& width, const unsigned int& height, const Color& color);
        Image(const Image&) = delete;
        Image& operator=(const Image&) const = delete; 
        ~Image();

        const glm::ivec2& getSize() const;
        void setColor(const unsigned int& x, const unsigned int& y, const MyBase::Color& color);
        const Color& getColor(const unsigned int& x, const unsigned int& y) const;
        void save(const std::string& source);
    protected:
    private:
        glm::ivec2      __size;
        MyBase::Color*  __pixel;
    };
}
#endif