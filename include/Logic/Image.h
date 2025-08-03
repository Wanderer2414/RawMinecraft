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
        
        void setPosition(const glm::ivec2& position);
        const glm::ivec2& getSize() const;
        const glm::ivec2& getPosition() const;
        void setColor(const int& x, const int& y, const MyBase::Color& color);
        const Color& getColor(const int& x, const int& y) const;
        void save(const std::string& source);
    protected:
    private:
        glm::ivec2      __size, __position;
        MyBase::Color*  __pixel;
    };
}
#endif