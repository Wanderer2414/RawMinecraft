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

        void loadFont(const std::string& source);
        void DrawText(const std::string& text, const glm::vec2& position, const glm::vec3& color) const;
    private:
        GLuint              __textmap, __vertex;
        stbtt_bakedchar     __data[96];
        std::vector<int> __utf8_to_codepoint(const std::string& text) const;
    };
};
#endif