#ifndef TEXT_H
#define TEXT_H
#include "Color.h"
#include "Global.h"
#include "Font.h"
namespace MyBase {
    class Text {
    public:
        Text();
        Text(const Text& text) = delete;
        ~Text();
        Text& operator=(const Text& text) = delete;
        const std::string& getText() const;
        void setTextColor(const Color& color);
        void setFont(const Font& font);
        void setText(const std::string& text);
        void setScale(const glm::vec2& scale);
        void draw() const;
        glm::vec2 getSize() const;
        const Font& getFont() const;
    protected:
        void move(const glm::vec2& offset);
        void setPosition(const glm::vec2& position);
        void update();
        glm::vec2 getPosition() const;
    private:
        unsigned char __text_size;
        GLuint __VBO, __VAO, __COLOR, __POSITION, __SCALE;
        Color       __color;
        std::string __text;
        glm::vec2   __position, __scale, __size;
        Font*       __font;
    };
}
#endif