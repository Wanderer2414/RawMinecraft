#ifndef FONT_H
#define FONT_H
#include "Global.h"
#include <string>
#include <vector>
namespace MyBase {
    class Font {
    public:
        Font(const std::string& source = "");
        ~Font();
        glm::vec2 MeasureText(const std::string& text) const;
        void loadFont(const std::string& source);
        friend class Text;
    protected:
        void Bind() const;
        char* getBuffer(const std::string& text, glm::vec2& size) const;
    private:
        GLuint              __textmap;
        stbtt_bakedchar     __data[96];
        std::vector<int> __utf_to_codepoint(const std::string& text) const;
    };
};
#endif