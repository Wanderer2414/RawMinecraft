#ifndef FONT_H
#define FONT_H
#include "FlyweightStorage.h"
#include "Global.h"
#include <string>
#include <vector>
namespace MyBase {
    class FontCore;
    class Font: private FlyWeightObject {
    public:
        Font(const std::string& src = "");
        ~Font();
        friend class Text;
    protected:
        void Bind() const;
        char* getBuffer(const std::string& text, glm::vec2& size) const;
    private:
        FlyWeightCore* create(const std::string& src) const override;
    };
    class FontCore: public FlyWeightCore {
    public:
        FontCore(const FontCore&) = delete;
        FontCore(FontCore&& ) = delete;
        FontCore operator=(const FontCore& font) = delete;
        #define font_height 100
        #define font_resolution 2048
        
        friend class Font;
        ~FontCore();
    protected:
        void Bind() const;
        char* getBuffer(const std::string& text, glm::vec2& size) const;
    private:
        FontCore();
        GLuint              __textmap;
        stbtt_bakedchar     __data[96];
        std::vector<int> __utf_to_codepoint(const std::string& text) const;
        glm::vec2 MeasureText(const std::string& text) const;
    };
};
#endif